// Copyright (c) 2025 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include <unordered_map>
#include <vector>

class SevenSegmentDisplay final : public wxControl
{
public:
#define SEGMENT_LIST TOP, TOP_LEFT, TOP_RIGHT, MIDDLE, BOTTOM_LEFT, BOTTOM_RIGHT, BOTTOM
	enum Segment : uint8_t { SEGMENT_LIST };

	static inline std::unordered_map<char, std::vector<Segment>> s_digitSegmentMap = {
		{'0', {TOP, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, BOTTOM}},
		{'1', {TOP_RIGHT, BOTTOM_RIGHT}},
		{'2', {TOP, TOP_RIGHT, MIDDLE, BOTTOM_LEFT, BOTTOM}},
		{'3', {TOP, TOP_RIGHT, MIDDLE, BOTTOM_RIGHT, BOTTOM}},
		{'4', {TOP_LEFT, TOP_RIGHT, MIDDLE, BOTTOM_RIGHT}},
		{'5', {TOP, TOP_LEFT, MIDDLE, BOTTOM_RIGHT, BOTTOM}},
		{'6', {TOP, TOP_LEFT, MIDDLE, BOTTOM_LEFT, BOTTOM_RIGHT, BOTTOM}},
		{'7', {TOP, TOP_RIGHT, BOTTOM_RIGHT}},
		{'8', {TOP, TOP_LEFT, TOP_RIGHT, MIDDLE, BOTTOM_LEFT, BOTTOM_RIGHT, BOTTOM}},
		{'9', {TOP, TOP_LEFT, TOP_RIGHT, MIDDLE, BOTTOM_RIGHT}},
		{' ', {}}
	};

	explicit SevenSegmentDisplay(wxWindow* parent);
	[[nodiscard]] unsigned short GetValue() const { return m_value; }
	void SetValue(unsigned short value);
	[[nodiscard]] unsigned short GetDigitCount() const { return m_digitCount; }
	void SetDigitCount(unsigned short digitCount);
	[[nodiscard]] unsigned short GetDigitSpacing() const { return m_digitSpacing; }
	void SetDigitSpacing(unsigned short digitSpacing);
	[[nodiscard]] unsigned short GetSegmentThickness() const { return m_segmentThickness; }
	void SetSegmentThickness(unsigned short segmentThickness);
	[[nodiscard]] wxSize GetDigitSize() const { return m_digitSize; }
	void SetDigitSize(wxSize digitSize);
	[[nodiscard]] bool GetLeadingZerosVisible() const { return m_leadingZerosVisible; }
	void SetLeadingZerosVisible(bool leadingZerosVisible);

private:
	unsigned short m_value{ 0 };
	unsigned short m_digitCount{ 3 };
	unsigned short m_digitSpacing{ 5 };
	unsigned short m_segmentThickness{ 3 };
	wxSize m_digitSize{ 20, 41 };
	bool m_leadingZerosVisible{ false };

	void SevenSegmentDisplay_OnPaint(wxPaintEvent& event);
	[[nodiscard]] wxSize DoGetBestClientSize() const override;
};
