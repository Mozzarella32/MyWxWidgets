#pragma once
#include "pch.hpp"

#include "../submodules/Instrumentor/Instrumentor.hpp"

class ChartControl : public wxWindow
{
public:
	enum class ChartStyle {
		Line,
		Area
	};

	// wxFULL_REPAINT_ON_RESIZE needed for Windows
	ChartControl(wxWindow* parent, wxWindowID id = wxID_ANY);


	void SetValues(const std::vector<double>& Values);

	void SetTitle(const std::string& Title);

	void SetUnit(const std::string& Unit, const std::string& UnitFormat = "%.2f");

	void SetPen(const wxPen& Pen);

	void SetBrush(const wxBrush& Brush);

	void SetStyle(const ChartStyle& style);

protected:
	std::vector<double> Values;

	bool HasChanged = true;
	std::string Title;
	std::string Unit;
	std::string UnitFormat = "%.2f";
	wxPen Pen;
	wxBrush Brush;
	size_t NumberValues;
	wxSize MySize = { -1,-1 };
	int segmentCount;
	double rangeLow;
	double rangeHigh;
	ChartStyle style = ChartStyle::Line;

private:

	wxBitmap Backround;
	//std::vector<wxPoint2DDouble> pointArray;
	std::vector<wxPoint> pointArray;
	wxAffineMatrix2D valueToChartArea;

	void UpdateBackround();

	void UpdatePointArray();

protected:
	virtual void UpdateHighLow();

private:
	void OnPaint(wxPaintEvent& evt);

protected:
	std::tuple<int, double, double> calculateChartSegmentCountAndRange(double origLow, double origHigh);
};

class FixedHeightChartCtrl :public ChartControl {
private:
	double UserLowBound = -1;
	double UserHighBound = -1;
protected:
	void UpdateHighLow() override;
public:
	static constexpr double DefaultLowBound = -1;
	static constexpr double DefaultHighBound = -1;

	FixedHeightChartCtrl(wxWindow* parent, wxWindowID id = wxID_ANY);

	void SetLowBound(const double& d);
	void SetHighBound(const double& d);

};

template <typename ChartDesendent>
	requires std::derived_from<ChartDesendent, ChartControl>
class ProgressivChartCtrl : public ChartDesendent {
private:
public:
	ProgressivChartCtrl(wxWindow* parent, wxWindowID id = wxID_ANY)
		: ChartDesendent(parent, id) {}

	void AddValue(double Value) {
		PROFILE_FUNKTION;
		while (ChartDesendent::Values.size() >= ChartDesendent::NumberValues && ChartDesendent::NumberValues != 0)
			ChartDesendent::Values.erase(ChartDesendent::Values.begin());
		ChartDesendent::Values.push_back(Value);
		ChartDesendent::UpdateHighLow();
	}

	void SetNumberValues(const size_t& NumberValues) {
		PROFILE_FUNKTION;
		ChartDesendent::NumberValues = NumberValues;
		ChartDesendent::HasChanged = true;
	}
};

//
//template <typename ChartDesendent>
//	requires std::derived_from<ChartDesendent, ChartControl>
//class OnlyAddChartCtrl : public ChartDesendent {
//private:
//public:
//	OnlyAddChartCtrl(wxWindow* parent, wxWindowID id = wxID_ANY)
//		: ChartDesendent(parent, id) {}
//
//	void AddValue(double Value) {
//		PROFILE_FUNKTION;
//		
//		ChartDesendent::Values.push_back(Value);
//		NumberValues = Values.size();
//
//		ChartDesendent::UpdateHighLow();
//	}
//
//	void ResetValues(){
//		ChartDesendent::SetValues({});
//	}
//
//};
