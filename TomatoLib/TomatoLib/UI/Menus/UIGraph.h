#pragma once
#ifndef __UIGRAPH_H__
#define __UIGRAPH_H__

#include "../UIBase.h"

#define GRAPH_COLOR_NET 0
#define GRAPH_COLOR_ENT 1
#define GRAPH_COLOR_UI 2
#define GRAPH_COLOR_ASYNC 3

namespace TomatoLib {
	struct UIGraphLineData {
		int Category;
		float MS;
	};
	class UIGraph : public UIBase {
	public:
		UIGraph(UIBase* parent);

		std::vector<Color> CatColors;
		std::vector<std::string> CatNames;
		std::vector<std::vector<UIGraphLineData>> Lines;
		std::vector<UIGraphLineData> TempData;

		int Offset;

		void InsertData(int category, float ms);
		void SetupCategory(int cat, std::string name);

		virtual void Draw(Render& p) override;
		virtual void Update() override;

		virtual void InvalidateLayout() override;
	};
}

#endif