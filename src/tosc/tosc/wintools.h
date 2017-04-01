#ifndef WINTOOLS_H
#define WINTOOLS_H

#include <QWidget>

/* 窗口标题栏组件项 */
struct TitleBarWidgetItem
{
	TitleBarWidgetItem(QWidget* pWidget, int nAlign = Qt::AlignRight,
		int nSpacing = 0, bool bAutoFillRest = false)
		: widget(pWidget), widgetAlign(nAlign), spacing(nSpacing), autoFillRest(bAutoFillRest)
	{}

	QWidget* widget;
	int		widgetAlign;
	int		spacing = 0;
	bool	autoFillRest;	// 是否自动占满剩余空间,注意:只能有一个组件设置为true
};
typedef QList<TitleBarWidgetItem> TitleBarWidgetList;


#endif	// WINTOOLS_H