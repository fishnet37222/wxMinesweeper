// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include <vector>
#include <random>

class MineField final : public wxControl
{
public:
	struct Cell
	{
		bool isMine{};
		bool isFlagged{};
		bool isRevealed{};
		bool wasClicked{};
		unsigned short adjacentMineCount{};
	};

	explicit MineField(wxWindow* parent);
	[[nodiscard]] wxSize GetFieldSize() const { return m_fieldSize; }
	void SetFieldSize(wxSize size);
	[[nodiscard]] wxSize GetCellSize() const { return m_cellSize; }
	void SetCellSize(wxSize size);
	[[nodiscard]] int GetMineCount() const { return m_mineCount; }

private:
	static std::mt19937 m_rng;
	std::uniform_int_distribution<> m_distX{};
	std::uniform_int_distribution<> m_distY{};

	wxSize m_fieldSize{ 9,9 };
	wxSize m_cellSize{ 25, 25 };
	std::vector<std::vector<Cell>> m_field{};
	int m_mineCount{ 10 };
	wxBitmap m_bmpFlag;
	wxBitmap m_bmpExplosion;
	std::vector<wxPoint> m_mineLocations;
	bool m_mouseLeftDown{ false };
	wxPoint m_hoveredCell{};

	void MineField_OnPaint(wxPaintEvent& event);
	[[nodiscard]] wxSize DoGetBestClientSize() const override;
	void InitializeField();
	void InitializeBitmaps();
	void MineField_OnRightUp(wxMouseEvent& event);
	void MineField_OnLeftUp(wxMouseEvent& event);
	wxPoint GetFieldCoordinatesFromClientCoordinates(const wxMouseEvent& event);
	void RevealCells(wxPoint point);
	void MineField_OnLeftDoubleClick(wxMouseEvent& event);
	void MineField_OnLeftDown(wxMouseEvent& event);
	void MineField_OnMouseMove(wxMouseEvent& event);
};
