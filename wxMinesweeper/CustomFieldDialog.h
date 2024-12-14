// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>

class CustomFieldDialog final : public wxDialog
{
public:
	CustomFieldDialog(wxWindow* parent, int customFieldWidth, int customFieldHeight, int customMineCount);
	[[nodiscard]] int GetCustomWidth() const;
	[[nodiscard]] int GetCustomHeight() const;
	[[nodiscard]] int GetCustomMineCount() const;

private:
	wxTextCtrl* m_txtCustomFieldWidth{};
	wxTextCtrl* m_txtCustomFieldHeight{};
	wxTextCtrl* m_txtCustomMineCount{};
};
