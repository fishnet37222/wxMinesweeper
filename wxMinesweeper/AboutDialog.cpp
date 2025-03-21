// Copyright (c) 2025 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "AboutDialog.h"

#include "LICENSE-2.0-html.h"

AboutDialog::AboutDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "About Minesweeper", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	wxDialog::SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));

	auto* szrMainOuter = new wxBoxSizer(wxHORIZONTAL);
	szrMainOuter->AddSpacer(12);
	auto* szrMainInner = new wxBoxSizer(wxVERTICAL);
	szrMainInner->AddSpacer(12);

	szrMainInner->Add(new wxStaticText(this, wxID_ANY, "Minesweeper 1.0.0"), wxSizerFlags(0).CenterHorizontal());

	szrMainInner->AddSpacer(5);

	szrMainInner->Add(new wxStaticText(this, wxID_ANY, "Copyright \u00a9 2025 FishNeSoft"), wxSizerFlags(0).CenterHorizontal());

	szrMainInner->AddSpacer(5);

	szrMainInner->Add(new wxStaticText(this, wxID_ANY, "Licensed under the Apache License, version 2.0"), wxSizerFlags(0).CenterHorizontal());

	szrMainInner->AddSpacer(5);

	szrMainInner->Add(new wxStaticText(this, wxID_ANY, "This app uses the WebView2 component from Microsoft"), wxSizerFlags(0).CenterHorizontal());

	szrMainInner->AddSpacer(5);

	m_wvLicense = wxWebView::New(this, wxID_ANY);
	m_wvLicense->SetMaxSize({ 600, 300 });
	m_wvLicense->SetMinSize({ 600, 300 });
	m_wvLicense->SetPage(license_html, wxEmptyString);
	m_wvLicense->Bind(wxEVT_WEBVIEW_NAVIGATING, &AboutDialog::WvLicense_OnNavigating, this);
	szrMainInner->Add(m_wvLicense, wxSizerFlags(1).Expand());

	szrMainInner->AddSpacer(5);

	auto* szrButton = new wxBoxSizer(wxHORIZONTAL);
	szrButton->AddStretchSpacer(1);
	szrButton->AddSpacer(5);

	m_btnClose = new wxButton(this, wxID_CLOSE);
	szrButton->Add(m_btnClose, wxSizerFlags(0));

	szrMainInner->Add(szrButton, wxSizerFlags(0).Expand());

	szrMainInner->AddSpacer(12);
	szrMainOuter->Add(szrMainInner, wxSizerFlags(1).Expand());
	szrMainOuter->AddSpacer(12);
	SetSizerAndFit(szrMainOuter);

	CenterOnParent();
	SetAffirmativeId(wxID_CLOSE);
	SetEscapeId(wxID_CLOSE);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
// ReSharper disable once CppMemberFunctionMayBeStatic
void AboutDialog::WvLicense_OnNavigating(wxWebViewEvent& event)
{
	if (event.GetURL().StartsWith("http://") || event.GetURL().StartsWith("https://"))
	{
		wxLaunchDefaultBrowser(event.GetURL());
		event.Veto();
	}
}
