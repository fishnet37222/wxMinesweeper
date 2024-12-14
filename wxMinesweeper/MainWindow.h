// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include "SevenSegmentDisplay.h"
#include "MineField.h"

class MainWindow final : public wxFrame
{
public:
	MainWindow();

private:
	wxMenuBar* m_menuBar{};
	SevenSegmentDisplay* m_ssdMinesLeft{};
	SevenSegmentDisplay* m_ssdElapsedTime{};
	wxBitmapButton* m_btnNewGame{};
	MineField* m_mineField{};
	int m_gameDifficulty{};

	void MenuBar_OnItemSelect(wxCommandEvent& event);
	void MineField_OnLeftDown(wxMouseEvent& event);
	void MineField_OnLeftUp(wxMouseEvent& event);
	void MainWindow_OnClose(wxCloseEvent& event);
};
