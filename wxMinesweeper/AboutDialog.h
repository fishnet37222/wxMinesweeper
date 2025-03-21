// Copyright (c) 2025 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include <wx/webview.h>

class AboutDialog final : public wxDialog
{
public:
	explicit AboutDialog(wxWindow* parent);

private:
	wxWebView* m_wvLicense{};
	wxButton* m_btnClose{};

	void WvLicense_OnNavigating(wxWebViewEvent& event);
};
