// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "MineField.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <sstream>
#include <stack>

#include "Bitmaps/flag-small.xpm"
#include "Bitmaps/explosion-small.xpm"

std::mt19937 MineField::m_rng{ std::random_device()() };

MineField::MineField(wxWindow* parent)
{
	wxControl::SetBackgroundStyle(wxBG_STYLE_PAINT);
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE);
	InitializeField();
	InitializeBitmaps();
	wxControl::SetBackgroundColour(wxTheColourDatabase->Find("LIGHT GREY"));
	wxControl::SetForegroundColour(wxTheColourDatabase->Find("BLACK"));
	Bind(wxEVT_PAINT, &MineField::MineField_OnPaint, this);
	Bind(wxEVT_RIGHT_UP, &MineField::MineField_OnRightUp, this);
	Bind(wxEVT_LEFT_UP, &MineField::MineField_OnLeftUp, this);
	Bind(wxEVT_LEFT_DCLICK, &MineField::MineField_OnLeftDoubleClick, this);
	Bind(wxEVT_LEFT_DOWN, &MineField::MineField_OnLeftDown, this);
	Bind(wxEVT_MOTION, &MineField::MineField_OnMouseMove, this);
}

void MineField::MineField_OnPaint([[maybe_unused]] wxPaintEvent& event)
{
	const wxAutoBufferedPaintDC dc(this);
	const std::unique_ptr<wxGraphicsContext> gc{ wxGraphicsContext::Create(dc) };

	gc->SetBrush(GetBackgroundColour());
	gc->SetPen(GetBackgroundColour());
	gc->DrawRectangle(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());

	const auto shadowColor = GetBackgroundColour().ChangeLightness(50);
	const auto highlightColor = GetBackgroundColour().ChangeLightness(150);

	for (auto cellY = 0; cellY < m_fieldSize.GetHeight(); cellY++)
	{
		const auto row = m_field[cellY];
		const auto cellOriginY = m_cellSize.GetHeight() * cellY;
		for (auto cellX = 0; cellX < m_fieldSize.GetWidth(); cellX++)
		{
			const auto& [isMine, isFlagged, isRevealed, wasClicked, adjacentMineCount] = row[cellX];
			const auto cellOriginX = m_cellSize.GetWidth() * cellX;

			if (const auto isHoveredCell = m_mouseLeftDown && cellX == m_hoveredCell.x && cellY == m_hoveredCell.y; isRevealed || (isHoveredCell && !isFlagged))
			{
				gc->SetPen(GetForegroundColour());
				gc->SetBrush(GetBackgroundColour());
				if (wasClicked && !isHoveredCell)
				{
					gc->SetBrush(wxTheColourDatabase->Find("RED"));
				}
				gc->DrawRectangle(cellOriginX, cellOriginY, m_cellSize.GetWidth(), m_cellSize.GetHeight());

				if (!isHoveredCell)
				{
					if (isMine)
					{
						gc->DrawBitmap(m_bmpExplosion, cellOriginX, cellOriginY, m_cellSize.GetWidth(), m_cellSize.GetHeight());
					}
					else if (adjacentMineCount > 0)
					{
						const auto text = std::to_string(adjacentMineCount);
						double textWidth, textHeight;
						gc->SetFont(GetFont().Bold(), GetForegroundColour());
						gc->GetTextExtent(text, &textWidth, &textHeight, nullptr, nullptr);
						const auto textOriginX = cellOriginX + (m_cellSize.GetWidth() / 2.0 - textWidth / 2.0);
						const auto textOriginY = cellOriginY + (m_cellSize.GetHeight() / 2.0 - textHeight / 2.0);
						gc->DrawText(text, textOriginX, textOriginY);
					}
				}
			}
			else
			{
				for (auto i = 0; i < 3; i++)
				{
					gc->SetPen(highlightColor);
					gc->StrokeLine(cellOriginX + i, cellOriginY + i, cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + i);
					gc->StrokeLine(cellOriginX + i, cellOriginY + i, cellOriginX + i, cellOriginY + m_cellSize.GetHeight() - i);
					gc->SetPen(shadowColor);
					gc->StrokeLine(cellOriginX + i, cellOriginY + m_cellSize.GetHeight() - i, cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + m_cellSize.GetHeight() - i);
					gc->StrokeLine(cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + i, cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + m_cellSize.GetHeight() - i);
				}

				if (isFlagged)
				{
					gc->DrawBitmap(m_bmpFlag, cellOriginX + 3, cellOriginY + 3, m_cellSize.GetWidth() - 6, m_cellSize.GetHeight() - 6);
				}
			}
		}
	}
}

wxSize MineField::DoGetBestClientSize() const
{
	return { m_cellSize.GetWidth() * m_fieldSize.GetWidth() + 1, m_cellSize.GetHeight() * m_fieldSize.GetHeight() + 1 };
}

void MineField::InitializeField()
{
	const std::vector<Cell> row{ static_cast<size_t>(m_fieldSize.GetWidth()) };
	m_field = std::vector{ static_cast<size_t>(m_fieldSize.GetHeight()), row };

	m_distX = std::uniform_int_distribution<>{ 0, m_fieldSize.GetWidth() - 1 };
	m_distY = std::uniform_int_distribution<>{ 0, m_fieldSize.GetHeight() - 1 };

	m_mineLocations.clear();

	for (auto i = 0; i < m_mineCount; i++)
	{
		int x, y;
		do
		{
			x = m_distX(m_rng);
			y = m_distY(m_rng);
		} while (m_field[y][x].isMine);
		m_field[y][x].isMine = true;
		m_mineLocations.emplace_back(x, y);

		for (auto y1 = y - 1; y1 <= y + 1; y1++)
		{
			if (y1 < 0 || y1 >= m_fieldSize.GetHeight()) continue;
			for (auto x1 = x - 1; x1 <= x + 1; x1++)
			{
				if (x1 < 0 || x1 >= m_fieldSize.GetWidth()) continue;
				if (x1 == x && y1 == y) continue;
				m_field[y1][x1].adjacentMineCount++;
			}
		}
	}

#ifdef _DEBUG
	for (auto y = 0; y < m_fieldSize.GetHeight(); y++)
	{
		std::wstringstream ss;
		const auto tmpRow = m_field[y];
		for (auto x = 0; x < m_fieldSize.GetWidth(); x++)
		{
			ss << (tmpRow[x].isMine ? L'*' : L'-');
		}
		ss << L'\n';
		OutputDebugString(ss.str().c_str());
	}
#endif
}

void MineField::SetFieldSize(const wxSize size)
{
	if (m_fieldSize == size) return;

	m_fieldSize = size;
	InitializeField();
	InvalidateBestSize();
	Refresh();
}

void MineField::SetCellSize(const wxSize size)
{
	if (m_cellSize == size) return;

	m_cellSize = size;
	InitializeBitmaps();
	InvalidateBestSize();
	Refresh();
}

void MineField::InitializeBitmaps()
{
	m_bmpExplosion = wxBitmap(explosion_small);
	wxBitmap::Rescale(m_bmpExplosion, m_cellSize);
	m_bmpFlag = wxBitmap(flag_small);
	wxBitmap::Rescale(m_bmpFlag, m_cellSize);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void MineField::MineField_OnRightUp(wxMouseEvent& event)
{
	const auto gridCoords = GetFieldCoordinatesFromClientCoordinates(event);

	if (gridCoords.x < 0 || gridCoords.x > m_fieldSize.GetWidth() || gridCoords.y < 0 || gridCoords.y > m_fieldSize.GetHeight()) return;

	auto& [isMine, isFlagged, isRevealed, wasClicked, adjacentMineCount] = m_field[gridCoords.y][gridCoords.x];

	if (isRevealed) return;

	isFlagged = !isFlagged;

	Refresh();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void MineField::MineField_OnLeftUp(wxMouseEvent& event)
{
	event.Skip();
	ReleaseMouse();
	m_mouseLeftDown = false;

	const auto gridCoords = GetFieldCoordinatesFromClientCoordinates(event);

	if (gridCoords.x < 0 || gridCoords.x >= m_fieldSize.GetWidth() || gridCoords.y < 0 || gridCoords.y >= m_fieldSize.GetHeight()) return;

	if (const auto& [isMine, isFlagged, isRevealed, wasClicked, adjacentMineCount] = m_field[gridCoords.y][gridCoords.x]; isRevealed || isFlagged) return;

	RevealCells(gridCoords);

	Refresh();
}

wxPoint MineField::GetFieldCoordinatesFromClientCoordinates(const wxMouseEvent& event)
{
	const wxWindowDC dc(this);
	const auto logicalPosition = event.GetLogicalPosition(dc);

	auto gridX = abs(logicalPosition.x) / m_cellSize.GetWidth();
	auto gridY = abs(logicalPosition.y) / m_cellSize.GetHeight();

	if (logicalPosition.x < 0)
	{
		gridX = -1;
	}

	if (logicalPosition.y < 0)
	{
		gridY = -1;
	}

	return { gridX, gridY };
}

void MineField::RevealCells(const wxPoint point)
{
	std::stack<wxPoint> cellsToReveal;

	cellsToReveal.push(point);

	bool mineRevealed = false;

	while (!cellsToReveal.empty())
	{
		const auto cellToReveal = cellsToReveal.top();
		cellsToReveal.pop();

		m_field[cellToReveal.y][cellToReveal.x].isRevealed = true;

		if (m_field[cellToReveal.y][cellToReveal.x].isMine)
		{
			mineRevealed = true;
			m_field[cellToReveal.y][cellToReveal.x].wasClicked = true;
			break;
		}

		if (m_field[cellToReveal.y][cellToReveal.x].adjacentMineCount > 0) continue;

		for (auto y1 = cellToReveal.y - 1; y1 <= cellToReveal.y + 1; y1++)
		{
			if (y1 < 0 || y1 >= m_fieldSize.GetHeight()) continue;
			for (auto x1 = cellToReveal.x - 1; x1 <= cellToReveal.x + 1; x1++)
			{
				if (x1 < 0 || x1 >= m_fieldSize.GetWidth()) continue;
				if (x1 == cellToReveal.x && y1 == cellToReveal.y) continue;
				if (m_field[y1][x1].isRevealed || m_field[y1][x1].isFlagged) continue;
				cellsToReveal.emplace(x1, y1);
			}
		}
	}

	if (!mineRevealed) return;

	for (const auto& mineLocation : m_mineLocations)
	{
		m_field[mineLocation.y][mineLocation.x].isRevealed = true;
	}
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void MineField::MineField_OnLeftDoubleClick(wxMouseEvent& event)
{
	const auto gridCoords = GetFieldCoordinatesFromClientCoordinates(event);

	if (!m_field[gridCoords.y][gridCoords.x].isRevealed) return;

	auto adjacentFlags = 0;
	for (auto y = gridCoords.y - 1; y <= gridCoords.y + 1; y++)
	{
		if (y < 0 || y >= m_fieldSize.GetHeight()) continue;
		for (auto x = gridCoords.x - 1; x <= gridCoords.x + 1; x++)
		{
			if (x < 0 || x >= m_fieldSize.GetWidth()) continue;
			if (x == gridCoords.x && y == gridCoords.y) continue;
			if (m_field[y][x].isFlagged) adjacentFlags++;
		}
	}

	if (adjacentFlags != m_field[gridCoords.y][gridCoords.x].adjacentMineCount) return;

	for (auto y = gridCoords.y - 1; y <= gridCoords.y + 1; y++)
	{
		if (y < 0 || y >= m_fieldSize.GetHeight()) continue;
		for (auto x = gridCoords.x - 1; x <= gridCoords.x + 1; x++)
		{
			if (x < 0 || x >= m_fieldSize.GetWidth()) continue;
			if (x == gridCoords.x && y == gridCoords.y) continue;
			if (m_field[y][x].isRevealed || m_field[y][x].isFlagged) continue;
			RevealCells({ x,y });
		}
	}
	Refresh();
}

void MineField::MineField_OnLeftDown(wxMouseEvent& event)
{
	event.Skip();
	CaptureMouse();
	m_mouseLeftDown = true;
	m_hoveredCell = GetFieldCoordinatesFromClientCoordinates(event);
	Refresh();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void MineField::MineField_OnMouseMove(wxMouseEvent& event)
{
	m_hoveredCell = GetFieldCoordinatesFromClientCoordinates(event);
	Refresh();
}
