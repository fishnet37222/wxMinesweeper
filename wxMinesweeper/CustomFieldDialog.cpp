// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "CustomFieldDialog.h"
#include <wx/gbsizer.h>

CustomFieldDialog::CustomFieldDialog(wxWindow* parent, const int customFieldWidth, const int customFieldHeight, const int customMineCount)
	: wxDialog(parent, wxID_ANY, "Custom Field")
{
	auto* szrMainOuter = new wxBoxSizer(wxHORIZONTAL);
	szrMainOuter->AddSpacer(12);

	auto* szrMainInner = new wxBoxSizer(wxVERTICAL);
	szrMainInner->AddSpacer(12);

	auto* szrTop = new wxGridBagSizer(5, 5);

	szrTop->Add(new wxStaticText(this, wxID_ANY, "Width"), wxGBPosition(0, 0), wxDefaultSpan, wxSizerFlags(0).CenterVertical().GetFlags());

	m_txtCustomFieldWidth = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
	szrTop->Add(m_txtCustomFieldWidth, wxGBPosition(0, 1));

	szrTop->Add(new wxStaticText(this, wxID_ANY, "Height"), wxGBPosition(1, 0), wxDefaultSpan, wxSizerFlags(0).CenterVertical().GetFlags());

	m_txtCustomFieldHeight = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
	szrTop->Add(m_txtCustomFieldHeight, wxGBPosition(1, 1));

	szrTop->Add(new wxStaticText(this, wxID_ANY, "Mines"), wxGBPosition(2, 0), wxDefaultSpan, wxSizerFlags(0).CenterVertical().GetFlags());

	m_txtCustomMineCount = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
	szrTop->Add(m_txtCustomMineCount, wxGBPosition(2, 1));

	szrTop->AddGrowableCol(0, 1);
	szrTop->SetFlexibleDirection(wxHORIZONTAL);

	szrMainInner->Add(szrTop, wxSizerFlags(1).Expand());

	szrMainInner->AddSpacer(5);

	auto* szrButtons = CreateButtonSizer(wxOK | wxCANCEL);
	szrMainInner->Add(szrButtons, wxSizerFlags(0).Expand());

	szrMainInner->AddSpacer(12);
	szrMainOuter->Add(szrMainInner, wxSizerFlags(1).Expand());

	szrMainOuter->AddSpacer(12);
	SetSizerAndFit(szrMainOuter);
	CenterOnParent();

	m_txtCustomFieldWidth->SetValue(std::to_string(customFieldWidth));
	m_txtCustomFieldHeight->SetValue(std::to_string(customFieldHeight));
	m_txtCustomMineCount->SetValue(std::to_string(customMineCount));
}

int CustomFieldDialog::GetCustomWidth() const
{
	return std::stoi(m_txtCustomFieldWidth->GetValue().ToStdString());
}

int CustomFieldDialog::GetCustomHeight() const
{
	return std::stoi(m_txtCustomFieldHeight->GetValue().ToStdString());
}

int CustomFieldDialog::GetCustomMineCount() const
{
	return std::stoi(m_txtCustomMineCount->GetValue().ToStdString());
}
