// Copyright (c) 2025 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include "SevenSegmentDisplay.h"

class MainWindow final : public wxFrame
{
public:
	MainWindow();

private:
	wxBitmapButton* m_btnNewGame{};
	wxMenuBar* m_menuBar{};
	SevenSegmentDisplay* m_ssdMinesLeft{};
	SevenSegmentDisplay* m_ssdTimeElapsed{};

	void MenuBar_OnItemSelect(wxCommandEvent& event);
};
