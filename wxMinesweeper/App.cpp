// Copyright (c) 2025 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "App.h"
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	if (!wxApp::OnInit()) return false;

	auto* mainWindow = new MainWindow();
	SetTopWindow(mainWindow);
	mainWindow->Show();

	return true;
}
