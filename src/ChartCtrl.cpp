#include "ChartCtrl.hpp"

// wxFULL_REPAINT_ON_RESIZE needed for Windows

ChartControl::ChartControl(wxWindow* parent, wxWindowID id) : wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
	PROFILE_FUNKTION;
	this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows
	this->SetBackgroundColour(wxSystemSettings::GetAppearance().IsDark() ? *wxBLACK : *wxWHITE);
	this->Bind(wxEVT_PAINT, &ChartControl::OnPaint, this);

	wxPen LinePen(*wxBLUE, 3, wxPENSTYLE_SOLID);
	LinePen.SetQuality(wxPenQuality::wxPEN_QUALITY_HIGH);
	this->Pen = LinePen;
	UpdateHighLow();
}

void ChartControl::SetValues(const std::vector<double>& Values) {
	PROFILE_FUNKTION;
	if (Values == this->Values)return;

	this->Values = Values;
	NumberValues = Values.size();

	UpdateHighLow();
}

void ChartControl::SetTitle(const std::string& Title) {
	PROFILE_FUNKTION;
	this->Title = Title;
	HasChanged = true;
}

void ChartControl::SetUnit(const std::string& Unit, const std::string& UnitFormat) {
	PROFILE_FUNKTION;
	this->Unit = Unit;
	this->UnitFormat = UnitFormat;
	HasChanged = true;
}

void ChartControl::SetPen(const wxPen& Pen) {
	PROFILE_FUNKTION;
	this->Pen = Pen;
}

void ChartControl::SetBrush(const wxBrush& Brush) {
	PROFILE_FUNKTION;
	this->Brush = Brush;
}

void ChartControl::SetStyle(const ChartStyle& style) {
	PROFILE_FUNKTION;
	this->style = style;
	HasChanged = true;
}

void ChartControl::UpdateBackround() {
	PROFILE_FUNKTION;

	Backround.CreateWithDIPSize(GetSize(), GetDPIScaleFactor());

	wxMemoryDC dc(Backround);

	dc.SetBackground(GetBackgroundColour());
	dc.Clear();

	std::unique_ptr<wxGraphicsContext> gc{ wxGraphicsContext::Create(dc) };

	wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize() * 2,
		wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	gc->SetFont(titleFont, wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);
	dc.SetFont(titleFont);

	// auto titleFontPointSize = titleFont.GetFractionalPointSize();

	double tw, th;
	gc->GetTextExtent(Title, &tw, &th);


	wxRect2DDouble fullArea{ 0, 0, static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight()) };

	const double marginX = fullArea.GetSize().GetWidth() / 8.0;
	const double marginTop = std::max(fullArea.GetSize().GetHeight() / 8.0, th);
	const double marginBottom = fullArea.GetSize().GetHeight() / 8.0;

	wxRect2DDouble chartArea = fullArea;
	chartArea.Inset(marginX, marginTop, marginX, marginBottom);

	gc->DrawText(Title, (fullArea.GetSize().GetWidth() - tw) / 2.0, (marginTop - th) / 2.0);

	wxAffineMatrix2D normalizedToChartArea{};
	normalizedToChartArea.Translate(chartArea.GetLeft(), chartArea.GetTop());
	normalizedToChartArea.Scale(chartArea.m_width, chartArea.m_height);

	double yValueSpan = rangeHigh - rangeLow;

	double yLinesCount = segmentCount + 1;

	wxAffineMatrix2D normalizedToValue{};
	normalizedToValue.Translate(0, rangeHigh);
	normalizedToValue.Scale(1, -1);
	normalizedToValue.Scale(static_cast<double>(NumberValues - 1), yValueSpan);

	static const wxPen LinePen(wxColor(128, 128, 128));
	static const wxPen LinePenZero(wxColor(128, 128, 128), 5);
	static const wxPen FramePen(wxColor(132, 132, 132), 5);

	gc->SetFont(*wxNORMAL_FONT, wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);

	const double UnitRightMargin = FromDIP(5);

	for (int i = 0; i < yLinesCount; i++) {
		double normalizedLineY = static_cast<double>(i) / (yLinesCount - 1);

		auto lineStartPoint = normalizedToChartArea.TransformPoint({ 0, normalizedLineY });
		auto lineEndPoint = normalizedToChartArea.TransformPoint({ 1, normalizedLineY });

		double valueAtLineY = normalizedToValue.TransformPoint({ 0, normalizedLineY }).m_y;

		if (i == 0 || i == yLinesCount - 1)gc->SetPen(FramePen);
		else if (valueAtLineY == 0)gc->SetPen(LinePenZero);
		else gc->SetPen(LinePen);

		wxPoint2DDouble linePoints[] = { lineStartPoint, lineEndPoint };
		gc->StrokeLines(2, linePoints);

		auto text = wxString::Format(wxString(UnitFormat), valueAtLineY);
		text += Unit;
		text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetLeft() * 2 - UnitRightMargin);

		double tw, th;
		gc->GetTextExtent(text, &tw, &th);
		gc->DrawText(text, chartArea.GetLeft() - UnitRightMargin - tw, lineStartPoint.m_y - th / 2.0);
	}

	gc->SetPen(FramePen);

	wxPoint2DDouble leftHLinePoints[] = {
		normalizedToChartArea.TransformPoint({ 0, 0 }),
		normalizedToChartArea.TransformPoint({ 0, 1 }) };

	wxPoint2DDouble rightHLinePoints[] = {
		normalizedToChartArea.TransformPoint({ 1, 0 }),
		normalizedToChartArea.TransformPoint({ 1, 1 }) };

	gc->StrokeLines(2, leftHLinePoints);
	gc->StrokeLines(2, rightHLinePoints);

	pointArray.clear();
	pointArray.reserve(Values.size());

	wxAffineMatrix2D valueToNormalized = normalizedToValue;
	valueToNormalized.Invert();
	valueToChartArea = normalizedToChartArea;
	valueToChartArea.Concat(valueToNormalized);

	HasChanged = false;
}

void ChartControl::UpdatePointArray() {
	PROFILE_FUNKTION;
	pointArray.clear();

	if (style == ChartStyle::Area) {
		const auto& Start = valueToChartArea.TransformPoint({ 0,rangeLow });
		pointArray.push_back(wxPoint(Start.m_x, Start.m_y));
	}

	for (size_t i = 0; i < Values.size(); i++) {
		const auto& Point = valueToChartArea.TransformPoint({ static_cast<double>(i), Values[i] });
		pointArray.push_back(wxPoint(Point.m_x, Point.m_y));
	}

	if (style == ChartStyle::Area) {
		auto End = valueToChartArea.TransformPoint({ static_cast<double>(Values.size()) - 1.0,rangeLow });
		pointArray.push_back(wxPoint(End.m_x, End.m_y));
		auto Start = valueToChartArea.TransformPoint({ 0,rangeLow });
		pointArray.push_back(wxPoint(Start.m_x, Start.m_y));
	}
}

void ChartControl::UpdateHighLow() {
	PROFILE_FUNKTION;

	double lowValue = Values.size() == 0 ? 0 : *std::min_element(Values.begin(), Values.end());
	double highValue = Values.size() == 0 ? 0 : *std::max_element(Values.begin(), Values.end());

	auto [newsegmentCount, newrangeLow, newrangeHigh] = calculateChartSegmentCountAndRange(lowValue, highValue);

	if (newsegmentCount != segmentCount || newrangeLow != rangeLow || newrangeHigh != rangeHigh) {
		segmentCount = newsegmentCount;
		rangeLow = newrangeLow;
		rangeHigh = newrangeHigh;
		HasChanged = true;
	}
}

void ChartControl::OnPaint([[maybe_unused]]wxPaintEvent& evt) {
	PROFILE_FUNKTION;

	if (MySize != GetSize()) {
		MySize = GetSize();
		HasChanged = true;
	}
	PROFILE_SCOPE_ID_START("Create dc and gc", 0);
	wxAutoBufferedPaintDC dc(this);
	PROFILE_SCOPE_ID_END(0);


	if (HasChanged) UpdateBackround();
	UpdatePointArray();

	PROFILE_SCOPE_ID_START("Copy Backround", 1);
	dc.DrawBitmap(Backround, { 0,0 });
	PROFILE_SCOPE_ID_END(1);

	if (style == ChartStyle::Area) {
		PROFILE_SCOPE_ID_START("Draw Areas", 3);
		dc.SetPen(Pen);
		dc.SetBrush(Brush);
		dc.DrawPolygon(pointArray.size(), pointArray.data());
		PROFILE_SCOPE_ID_END(3);
	}

	if (style == ChartStyle::Line) {
		PROFILE_SCOPE_ID_START("Draw Lines", 2);
		dc.SetPen(Pen);
		dc.DrawLines(pointArray.size(), pointArray.data());
		PROFILE_SCOPE_ID_END(2);
	}
}

std::tuple<int, double, double> ChartControl::calculateChartSegmentCountAndRange(double origLow, double origHigh) {
	PROFILE_FUNKTION;

	if (origHigh == origLow) {
		return{ 2,origLow,origHigh };
	}

	constexpr double rangeMults[] = { 0.2, 0.25, 0.5, 1.0, 2.0, 2.5, 5.0 };
	constexpr int maxSegments = 6;

	double magnitude = std::floor(std::log10(origHigh - origLow));

	for (auto r : rangeMults) {
		double stepSize = r * std::pow(10.0, magnitude);
		double low = std::floor(origLow / stepSize) * stepSize;
		double high = std::ceil(origHigh / stepSize) * stepSize;

		int segments = round((high - low) / stepSize);

		if (segments <= maxSegments) {
			return { segments, low, high };
		}
	}

	// return some defaults in case rangeMults and maxSegments are mismatched
	return { 10, origLow, origHigh };
}

void FixedHeightChartCtrl::UpdateHighLow() {
	PROFILE_FUNKTION;


	double lowValue = Values.size() == 0 ? 0 :
		(UserLowBound == DefaultLowBound) ?
		*std::min_element(Values.begin(), Values.end())
		: UserLowBound;

	double highValue = Values.size() == 0 ? 0 :
		(UserHighBound == DefaultHighBound) ?
		*std::max_element(Values.begin(), Values.end())
		: UserHighBound;

	auto [newsegmentCount, newrangeLow, newrangeHigh] = calculateChartSegmentCountAndRange(lowValue, highValue);

	if (newsegmentCount != segmentCount || newrangeLow != rangeLow || newrangeHigh != rangeHigh) {
		segmentCount = newsegmentCount;
		rangeLow = newrangeLow;
		rangeHigh = newrangeHigh;
		HasChanged = true;
	}
}

FixedHeightChartCtrl::FixedHeightChartCtrl(wxWindow* parent, wxWindowID id)
	: ChartControl(parent, id) {}

void FixedHeightChartCtrl::SetLowBound(const double& d) {
	this->UserLowBound = d;
	HasChanged = true;
}

void FixedHeightChartCtrl::SetHighBound(const double& d) {
	this->UserHighBound = d;
	HasChanged = true;
}
