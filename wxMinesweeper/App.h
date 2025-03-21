// Copyright (c) 2025 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>

class App final : public wxApp
{
public:
	bool OnInit() override;
};

wxDECLARE_APP(App);
