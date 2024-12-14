// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "MainWindow.h"
#include <wx/config.h>

#include "Bitmaps/smile-1.xpm"
#include "Bitmaps/smile-2.xpm"
#include "Bitmaps/smile-3.xpm"
#include "Bitmaps/smile-4.xpm"

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

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	wxFrame::SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));
	wxFrame::SetIcons(wxIconBundle("APP_ICON", nullptr));

	auto* szrMainOuter = new wxBoxSizer(wxHORIZONTAL);
	szrMainOuter->AddSpacer(12);

	m_menuBar = new wxMenuBar();

	auto* mnuGame = new wxMenu();
	mnuGame->Append(ID_GAME_NEW, "&New\tF2");
	mnuGame->AppendSeparator();
	mnuGame->AppendCheckItem(ID_GAME_BEGINNER, "&Beginner");
	mnuGame->AppendCheckItem(ID_GAME_INTERMEDIATE, "&Intermediate");
	mnuGame->AppendCheckItem(ID_GAME_EXPERT, "&Expert");
	mnuGame->AppendCheckItem(ID_GAME_CUSTOM, "&Custom...");
	mnuGame->AppendSeparator();
	mnuGame->Append(ID_GAME_BEST_TIMES, "Best &Times...");
	mnuGame->AppendSeparator();
	mnuGame->Append(ID_GAME_EXIT, "E&xit");
	m_menuBar->Append(mnuGame, "&Game");

	auto* mnuHelp = new wxMenu();
	mnuHelp->Append(ID_HELP_HOW_TO_PLAY, "How to &Play");
	mnuHelp->AppendSeparator();
	mnuHelp->Append(ID_HELP_ABOUT, "About &Minesweeper");
	m_menuBar->Append(mnuHelp, "&Help");
	m_menuBar->Bind(wxEVT_MENU, &MainWindow::MenuBar_OnItemSelect, this);

	wxFrame::SetMenuBar(m_menuBar);

	auto* szrMainInner = new wxBoxSizer(wxVERTICAL);
	szrMainInner->AddSpacer(5);

	auto* szrTop = new wxBoxSizer(wxHORIZONTAL);

	m_ssdMinesLeft = new SevenSegmentDisplay(this);
	szrTop->Add(m_ssdMinesLeft, wxSizerFlags(0).CenterVertical());

	szrTop->AddStretchSpacer(1);
	szrTop->AddSpacer(5);

	m_btnNewGame = new wxBitmapButton(this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(smile_1_xpm)));
	szrTop->Add(m_btnNewGame, wxSizerFlags(0).CenterVertical());

	szrTop->AddSpacer(5);
	szrTop->AddStretchSpacer(1);

	m_ssdElapsedTime = new SevenSegmentDisplay(this);
	szrTop->Add(m_ssdElapsedTime, wxSizerFlags(0).CenterVertical());

	szrMainInner->Add(szrTop, wxSizerFlags(0).Expand());

	szrMainInner->AddSpacer(5);

	m_mineField = new MineField(this);
	m_mineField->Bind(wxEVT_LEFT_DOWN, &MainWindow::MineField_OnLeftDown, this);
	m_mineField->Bind(wxEVT_LEFT_UP, &MainWindow::MineField_OnLeftUp, this);
	szrMainInner->Add(m_mineField, wxSizerFlags(0).CenterHorizontal());

	szrMainInner->AddSpacer(12);
	szrMainOuter->Add(szrMainInner, wxSizerFlags(1).Expand());

	szrMainOuter->AddSpacer(12);
	SetSizerAndFit(szrMainOuter);

	const auto* config = wxConfig::Get();
	const auto posX = config->ReadLong("MainWindowPosX", -1);
	// ReSharper disable once CppTooWideScopeInitStatement
	const auto posY = config->ReadLong("MainWindowPosY", -1);

	if (posX == -1 && posY == -1)
	{
	CenterOnScreen();
}
	else
	{
		SetPosition({ posX, posY });
	}

	Bind(wxEVT_CLOSE_WINDOW, &MainWindow::MainWindow_OnClose, this);
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

		case ID_GAME_NEW:
		case ID_GAME_BEGINNER:
		case ID_GAME_INTERMEDIATE:
		case ID_GAME_EXPERT:
		case ID_GAME_CUSTOM:
		case ID_GAME_BEST_TIMES:
		case ID_HELP_HOW_TO_PLAY:
		case ID_HELP_ABOUT:
		default:
			break;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainWindow::MineField_OnLeftDown([[maybe_unused]] wxMouseEvent& event)
{
	event.Skip();
	m_btnNewGame->SetBitmap(wxBitmapBundle::FromBitmap(wxBitmap(smile_4_xpm)));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainWindow::MineField_OnLeftUp([[maybe_unused]] wxMouseEvent& event)
{
	event.Skip();
	m_btnNewGame->SetBitmap(wxBitmapBundle::FromBitmap(wxBitmap(smile_1_xpm)));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MainWindow::MainWindow_OnClose([[maybe_unused]] wxCloseEvent& event)
{
	auto* config = wxConfig::Get();
	const auto pos = GetPosition();

	config->Write("MainWindowPosX", pos.x);
	config->Write("MainWindowPosY", pos.y);

	Destroy();
}
