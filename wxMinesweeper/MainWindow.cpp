// Copyright (c) 2025 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "MainWindow.h"
#include "AboutDialog.h"

#include "bitmaps/smile-1.xpm"
#include "bitmaps/smile-2.xpm"
#include "bitmaps/smile-3.xpm"
#include "bitmaps/smile-4.xpm"

enum MenuId : uint16_t
{
	ID_GAME_NEW = wxID_HIGHEST + 1,
	ID_GAME_BEGINNER,
	ID_GAME_INTERMEDIATE,
	ID_GAME_EXPERT,
	ID_GAME_CUSTOM,
	ID_GAME_BEST_TIMES,
	ID_GAME_EXIT,
	ID_HELP_HOW_TO_PLAY,
	ID_HELP_ABOUT
};

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	wxFrame::SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));
	wxFrame::SetIcons(wxIconBundle("APP_ICON", nullptr));

	m_menuBar = new wxMenuBar();

	auto* mnuGame = new wxMenu();
	mnuGame->Append(ID_GAME_NEW, "&New\tF2");
	mnuGame->AppendSeparator();
	mnuGame->AppendCheckItem(ID_GAME_BEGINNER, "&Beginner");
	mnuGame->AppendCheckItem(ID_GAME_INTERMEDIATE, "&Intermediate");
	mnuGame->AppendCheckItem(ID_GAME_EXPERT, "&Expert");
	mnuGame->AppendCheckItem(ID_GAME_CUSTOM, "Custom...");
	mnuGame->AppendSeparator();
	mnuGame->Append(ID_GAME_BEST_TIMES, "Best &Times...");
	mnuGame->AppendSeparator();
	mnuGame->Append(ID_GAME_EXIT, "E&xit");
	m_menuBar->Append(mnuGame, "&Game");

	auto* mnuHelp = new wxMenu();
	mnuHelp->Append(ID_HELP_HOW_TO_PLAY, "How to &Play");
	mnuHelp->AppendSeparator();
	mnuHelp->Append(ID_HELP_ABOUT, "&About Minesweeper");
	m_menuBar->Append(mnuHelp, "&Help");

	wxFrame::SetMenuBar(m_menuBar);
	m_menuBar->Bind(wxEVT_MENU, &MainWindow::MenuBar_OnItemSelect, this);

	auto* szrMainOuter = new wxBoxSizer(wxHORIZONTAL);
	szrMainOuter->AddSpacer(12);
	auto* szrMainInner = new wxBoxSizer(wxVERTICAL);
	szrMainInner->AddSpacer(5);

	auto* szrTop = new wxBoxSizer(wxHORIZONTAL);

	m_ssdMinesLeft = new SevenSegmentDisplay(this);
	szrTop->Add(m_ssdMinesLeft, wxSizerFlags(0).CenterVertical());

	szrTop->AddSpacer(5);
	szrTop->AddStretchSpacer(1);

	m_btnNewGame = new wxBitmapButton(this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(smile_1_xpm)));
	szrTop->Add(m_btnNewGame, wxSizerFlags(0).CenterVertical());

	szrTop->AddStretchSpacer(1);
	szrTop->AddSpacer(5);

	m_ssdTimeElapsed = new SevenSegmentDisplay(this);
	szrTop->Add(m_ssdTimeElapsed, wxSizerFlags(0).CenterVertical());

	szrMainInner->Add(szrTop, wxSizerFlags(0).Expand());

	szrMainInner->AddSpacer(12);
	szrMainOuter->Add(szrMainInner, wxSizerFlags(1).Expand());
	szrMainOuter->AddSpacer(12);
	SetSizerAndFit(szrMainOuter);

	CenterOnScreen();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void MainWindow::MenuBar_OnItemSelect(wxCommandEvent& event)
{
	switch (event.GetId())
	{
		case ID_GAME_EXIT:
		{
			Close();

			break;
		}

		case ID_HELP_ABOUT:
		{
			AboutDialog dlg(this);
			dlg.ShowModal();

			break;
		}

		default:
			break;
	}
}
