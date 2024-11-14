// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>

class MainWindow final : public wxFrame
{
public:
	MainWindow();

private:
	wxMenuBar* m_menuBar{};

	void MenuBar_OnItemSelect(wxCommandEvent& event);
};
