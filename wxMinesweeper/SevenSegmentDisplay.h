// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include <vector>
#include <unordered_map>

#define SEGMENT_LIST TOP,TOP_LEFT,TOP_RIGHT,MIDDLE,BOTTOM_LEFT,BOTTOM_RIGHT,BOTTOM
enum Segment : uint8_t { SEGMENT_LIST };

class SevenSegmentDisplay final : public wxControl
{
public:
	explicit SevenSegmentDisplay(wxWindow* parent);
	[[nodiscard]] unsigned int GetValue() const { return m_value; }
	void SetValue(unsigned int value);
	[[nodiscard]] uint8_t GetDigitCount() const { return  m_digitCount; }
	void SetDigitCount(uint8_t digitCount);
	[[nodiscard]] uint8_t GetSegmentThickness() const { return  m_segmentThickness; }
	void SetSegmentThickness(uint8_t segmentThickness);
	[[nodiscard]] uint8_t GetDigitSpacing() const { return m_digitSpacing; }
	void SetDigitSpacing(uint8_t digitSpacing);
	[[nodiscard]] wxSize GetDigitSize() const { return m_digitSize; }
	void SetDigitSize(const wxSize& digitSize);
	[[nodiscard]] bool GetLeadingZerosVisible() const { return  m_leadingZerosVisible; }
	void SetLeadingZerosVisible(bool leadingZerosVisible);

private:
	static std::unordered_map<char, std::vector<Segment>> m_segments;
	uint8_t m_digitCount{ 3 };
	uint8_t m_digitSpacing{ 5 };
	uint8_t m_segmentThickness{ 3 };
	wxSize m_digitSize{ 20,40 };
	unsigned int m_value{ 0 };
	bool m_leadingZerosVisible{ false };

	void SevenSegmentDisplay_OnPaint(wxPaintEvent& event);
	[[nodiscard]] wxSize DoGetBestClientSize() const override;
};
