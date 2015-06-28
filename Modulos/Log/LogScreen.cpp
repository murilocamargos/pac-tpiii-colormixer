#include "../../App/Includes.h"

#include "LogScreen.h"
#include <wx/file.h>

#include "../../Controls/DirSelect.h"

BEGIN_EVENT_TABLE(LogScreen, wxDialog)
    EVT_CHOICE(CHOICE_DATE_OPR, LogScreen::OnChoiceOprDate)
    EVT_CHOICE(ALL_CHOICES, LogScreen::OnChoice)
    EVT_COMBOBOX(ALL_COMBO, LogScreen::OnCombo)
    EVT_DATE_CHANGED(ALL_DATE, LogScreen::OnDate)
    EVT_BUTTON(EXPORT, LogScreen::OnExport)
    EVT_CLOSE(LogScreen::OnExit)
END_EVENT_TABLE()

LogScreen::LogScreen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->db  = new SQLiteHandler();
    this->sql = new SQLHandler();

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* parentBS;
	parentBS = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* searchFieldsBS;
	searchFieldsBS = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* SFUsersBS;
	SFUsersBS = new wxBoxSizer( wxVERTICAL );

	SFULabel = new wxStaticText( this, wxID_ANY, _("User") + ":", wxDefaultPosition, wxDefaultSize, 0 );
	SFULabel->Wrap( -1 );
	SFUsersBS->Add( SFULabel, 0, wxALL, 5 );

	wxString SFUOperatorChoices[] = { "=", "!=" };
	int SFUOperatorNChoices = sizeof( SFUOperatorChoices ) / sizeof( wxString );
	SFUOperator = new wxChoice( this, ALL_CHOICES, wxDefaultPosition, wxSize( 120,-1 ), SFUOperatorNChoices, SFUOperatorChoices, 0 );
	SFUOperator->SetSelection( 0 );
	SFUsersBS->Add( SFUOperator, 0, wxALL, 5 );

	SFUsers = new wxComboBox( this, ALL_COMBO, wxEmptyString, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0 );

	SFUsers->Append(_("All"));
	this->db->Select(this->sql->Table("usuarios")->Column("user_id")->Column("nome"));
	for (unsigned int i = 0; i < this->db->rows.size(); i++)
        SFUsers->Append(this->db->rows[i]["user_id"] + ". " + wxString::FromUTF8Unchecked(wxString(this->db->rows[i]["nome"])));
	SFUsers->SetSelection(0);

	SFUsersBS->Add( SFUsers, 0, wxALL, 5 );


	searchFieldsBS->Add( SFUsersBS, 1, wxEXPAND, 5 );

	wxBoxSizer* SFConnectorBS1;
	SFConnectorBS1 = new wxBoxSizer( wxVERTICAL );


	SFConnectorBS1->Add( 0, 0, 1, wxEXPAND, 5 );

	wxString SFConnector1Choices[] = { _("And"), _("Or") };
	int SFConnector1NChoices = sizeof( SFConnector1Choices ) / sizeof( wxString );
	SFConnector1 = new wxChoice( this, ALL_CHOICES, wxDefaultPosition, wxSize( 50,30 ), SFConnector1NChoices, SFConnector1Choices, 0 );
	SFConnector1->SetSelection( 0 );
	SFConnectorBS1->Add( SFConnector1, 0, wxALL, 5 );


	SFConnectorBS1->Add( 0, 0, 1, wxEXPAND, 5 );


	searchFieldsBS->Add( SFConnectorBS1, 1, wxEXPAND, 5 );

	wxBoxSizer* SFTypeBS;
	SFTypeBS = new wxBoxSizer( wxVERTICAL );

	SFTLabel = new wxStaticText( this, wxID_ANY, _("Type") + ":", wxDefaultPosition, wxDefaultSize, 0 );
	SFTLabel->Wrap( -1 );
	SFTypeBS->Add( SFTLabel, 0, wxALL, 5 );

	wxString SFTOperatorChoices[] = { "=", "!=" };
	int SFTOperatorNChoices = sizeof( SFTOperatorChoices ) / sizeof( wxString );
	SFTOperator = new wxChoice( this, ALL_CHOICES, wxDefaultPosition, wxSize( 120,-1 ), SFTOperatorNChoices, SFTOperatorChoices, 0 );
	SFTOperator->SetSelection( 0 );
	SFTypeBS->Add( SFTOperator, 0, wxALL, 5 );

	SFTypes = new wxComboBox( this, ALL_COMBO, wxEmptyString, wxDefaultPosition, wxSize( 120,-1 ), 0, NULL, 0 );

	SFTypes->Append(_("All"));
	this->db->Select(this->sql->Table("log_desc")->Column("log_id")->Column("descricao"));
	for (unsigned int i = 0; i < this->db->rows.size(); i++)
        SFTypes->Append(this->db->rows[i]["log_id"] + ". " + wxString::FromUTF8Unchecked(wxString(this->db->rows[i]["descricao"])));
	SFTypes->SetSelection(0);

	SFTypeBS->Add( SFTypes, 0, wxALL, 5 );


	searchFieldsBS->Add( SFTypeBS, 1, wxEXPAND, 5 );

	wxBoxSizer* SFConnectorBS2;
	SFConnectorBS2 = new wxBoxSizer( wxVERTICAL );


	SFConnectorBS2->Add( 0, 0, 1, wxEXPAND, 5 );

	wxString SFConnector2Choices[] = { _("And"), _("Or") };
	int SFConnector2NChoices = sizeof( SFConnector2Choices ) / sizeof( wxString );
	SFConnector2 = new wxChoice( this, ALL_CHOICES, wxDefaultPosition, wxSize( 50,30 ), SFConnector2NChoices, SFConnector2Choices, 0 );
	SFConnector2->SetSelection( 0 );
	SFConnectorBS2->Add( SFConnector2, 0, wxALL, 5 );


	SFConnectorBS2->Add( 0, 0, 1, wxEXPAND, 5 );


	searchFieldsBS->Add( SFConnectorBS2, 1, wxEXPAND, 5 );

	wxBoxSizer* SFDateBS;
	SFDateBS = new wxBoxSizer( wxVERTICAL );

	SFDLabel = new wxStaticText( this, wxID_ANY, _("Date") + ":", wxDefaultPosition, wxDefaultSize, 0 );
	SFDLabel->Wrap( -1 );
	SFDateBS->Add( SFDLabel, 0, wxALL, 5 );

	wxString SFDOperatorChoices[] = { _("Any"), "=", "!=", ">", "<", ">=", "<=", _("Between"), _("Not between") };
	int SFDOperatorNChoices = sizeof( SFDOperatorChoices ) / sizeof( wxString );
	SFDOperator = new wxChoice( this, CHOICE_DATE_OPR, wxDefaultPosition, wxSize( 180,-1 ), SFDOperatorNChoices, SFDOperatorChoices, 0 );
	SFDOperator->SetSelection( 0 );
	SFDateBS->Add( SFDOperator, 0, wxALL, 5 );

	wxBoxSizer* SFDateFieldsBS;
	SFDateFieldsBS = new wxBoxSizer( wxHORIZONTAL );

	SFDatePicker1 = new wxDatePickerCtrl( this, ALL_DATE, wxDefaultDateTime, wxDefaultPosition, wxSize( 85,-1 ), wxDP_DEFAULT|wxDP_DROPDOWN );
	SFDatePicker1->Enable( false );
	SFDateFieldsBS->Add( SFDatePicker1, 0, wxALL, 5 );

	SFDatePicker2 = new wxDatePickerCtrl( this, ALL_DATE, wxDefaultDateTime, wxDefaultPosition, wxSize( 85,-1 ), wxDP_DEFAULT|wxDP_DROPDOWN );
	SFDatePicker2->Enable( false );

	SFDateFieldsBS->Add( SFDatePicker2, 0, wxALL, 5 );


	SFDateBS->Add( SFDateFieldsBS, 1, wxEXPAND, 5 );


	searchFieldsBS->Add( SFDateBS, 1, wxEXPAND, 5 );


	parentBS->Add( searchFieldsBS, 1, wxEXPAND, 5 );

	logsTable = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 570,300 ), 0 );

	logsTable->AppendTextColumn(_("User"));
    logsTable->AppendTextColumn(_("Log Type"), wxDATAVIEW_CELL_INERT, 210);
    logsTable->AppendTextColumn(_("Info."), wxDATAVIEW_CELL_INERT, 120);
    logsTable->AppendTextColumn(_("Date"), wxDATAVIEW_CELL_INERT, 110);

    this->Populate(sql->Table("logs as l")
                      ->Join("log_desc as ls", "l.log_id = ls.log_id")
                      ->NaturalJoin("usuarios as u")
                      ->Column("u.nome")
                      ->Column("l.ocorrencia")
                      ->Column("l.descricao as info")
                      ->Column("ls.descricao"));

	parentBS->Add( logsTable, 0, wxALL, 5 );

    wxBoxSizer* btnBS;
	btnBS = new wxBoxSizer( wxHORIZONTAL );
	btnBS->Add( 0, 0, 1, wxEXPAND, 5 );

	btnExport = new wxButton( this, EXPORT, _("Export Log"), wxDefaultPosition, wxSize( -1,30 ), 0 );
	btnBS->Add( btnExport, 0, wxALL, 5 );


	parentBS->Add( btnBS, 1, wxEXPAND, 5 );

	this->SetSizer( parentBS );
	this->Layout();

	this->Centre( wxBOTH );
}

LogScreen::~LogScreen()
{
}

void LogScreen::OnExit(wxCloseEvent& event)
{
    Destroy();
}

void LogScreen::Populate(SQLHandler* sql)
{
    unsigned long date;

    this->logsTable->DeleteAllItems();

    this->db->Select(sql);
    wxVector<wxVariant> data;

    for (unsigned int i = 0; i < this->db->rows.size(); i++)
    {
        wxString(this->db->rows[i]["ocorrencia"]).ToULong(&date);

        data.clear();
        data.push_back(wxVariant(wxString::FromUTF8Unchecked(wxString(this->db->rows[i]["nome"]))));
        data.push_back(wxVariant(wxString::FromUTF8Unchecked(wxString(this->db->rows[i]["descricao"]))));
        data.push_back(wxVariant(wxString::FromUTF8Unchecked(wxString(this->db->rows[i]["info"]))));
        data.push_back(wxVariant(wxDateTime(static_cast<time_t>(date)).FormatISOCombined(' ')));
        this->logsTable->AppendItem(data);
    }
}

void LogScreen::Find()
{
    where USER, TYPE, DATE;

    /////////////////////////////////////////////////////////////
    wxString uid("");
    wxString uopr = this->SFUOperator->GetString(this->SFUOperator->GetSelection());
    if (this->SFUsers->GetSelection() != 0)
    {
        uid = this->SFUsers->GetString(this->SFUsers->GetSelection());
        uid = uid.SubString(0, uid.Find(".") - 1);
    }

    USER.col  = "u.user_id";
    USER.opr  = std::string(uopr.mb_str());
    USER.val1 = std::string(uid.mb_str());
    USER.val2 = "";
    USER.con  = "AND";
    /////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    wxString lid("");
    wxString lopr = this->SFTOperator->GetString(this->SFTOperator->GetSelection());
    wxString lcon = (this->SFConnector1->GetSelection() == 0) ? "AND" : "OR";
    if (this->SFTypes->GetSelection() != 0)
    {
        lid = this->SFTypes->GetString(this->SFTypes->GetSelection());
        lid = lid.SubString(0, lid.Find(".") - 1);
    }

    TYPE.col  = "l.log_id";
    TYPE.opr  = std::string(lopr.mb_str());
    TYPE.val1 = std::string(lid.mb_str());
    TYPE.val2 = "";
    TYPE.con  = std::string(lcon.mb_str());
    /////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    int dsopr = this->SFDOperator->GetSelection();
    wxString d1s(""), d2s("");
    wxDateTime d1 = this->SFDatePicker1->GetValue(), d2 = this->SFDatePicker2->GetValue();

    if (dsopr == 1 || dsopr == 2)
    {
        dsopr = 6 + dsopr;
        d2 = d1;
        d2.Add(wxTimeSpan::Hours(24));
    }
    else if (dsopr == 3 || dsopr == 6)
    {
        d1.Add(wxTimeSpan::Hours(24));
    }
    else if (dsopr == 7 || dsopr == 8)
    {
        d2.Add(wxTimeSpan::Hours(24));
    }

    if (this->SFDOperator->GetSelection() != 0)
        d1s = wxString::Format("%i", d1.GetTicks());
    if (dsopr == 7 || dsopr == 8)
        d2s = wxString::Format("%i", d2.GetTicks());

    wxString dopr = this->SFDOperator->GetString(dsopr);
    wxString dcon = (this->SFConnector2->GetSelection() == 0) ? "AND" : "OR";

    DATE.col  = "ocorrencia";
    DATE.opr  = std::string(dopr.mb_str());
    DATE.val1 = std::string(d1s.mb_str());
    DATE.val2 = std::string(d2s.mb_str());
    DATE.con  = std::string(dcon.mb_str());
    /////////////////////////////////////////////////////////////

    sql->Table("logs as l")
       ->Join("log_desc as ls", "l.log_id = ls.log_id")
       ->NaturalJoin("usuarios as u")
       ->Column("u.nome")
       ->Column("l.ocorrencia")
       ->Column("l.descricao as info")
       ->Column("ls.descricao")
       ->Where(USER)
       ->Where(TYPE)
       ->Where(DATE);

    this->Populate(sql);
}

void LogScreen::OnChoiceOprDate(wxCommandEvent& event)
{
    this->Find();
    this->SFDatePicker1->Enable(this->SFDOperator->GetSelection() > 0);
    this->SFDatePicker2->Enable(this->SFDOperator->GetSelection() == 7);
}

void LogScreen::OnChoice(wxCommandEvent& event)
{
    this->Find();
}

void LogScreen::OnCombo(wxCommandEvent& event)
{
    this->Find();
}

void LogScreen::OnDate(wxDateEvent& event)
{
    this->Find();
}

void LogScreen::OnExport(wxCommandEvent& event)
{
    wxString path = DirSelect::PathDlg();

    if (path == "")
    {
        wxMessageDialog dlg(this, _("You must select a path to save the file!"), _("Error"), wxICON_WARNING);
        dlg.ShowModal();
    }
    else
    {
        wxFile file(path + "/ColorMixer.log", wxFile::write);

        if (!file.IsOpened())
        {
            wxMessageDialog dlg(this, _("We couldn't create the file in this directory!"), _("Error"), wxICON_WARNING);
            dlg.ShowModal();
        }
        else
        {
            for (unsigned int i = 0; i < this->logsTable->GetItemCount(); i++)
            {
                wxString row("");
                row += "[" + this->logsTable->GetTextValue(i, 3) + "] ";
                row += "[" + this->logsTable->GetTextValue(i, 0) + "] ";
                row += "[" + this->logsTable->GetTextValue(i, 1) + "] ";
                row += this->logsTable->GetTextValue(i, 2) + "\r\n";
                file.Write(row);
            }

            wxMessageDialog dlg(this, _("The log file was saved!"), _("Success"), wxICON_INFORMATION);
            dlg.ShowModal();
        }

        file.Close();
    }
}