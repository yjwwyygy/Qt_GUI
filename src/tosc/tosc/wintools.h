#ifndef WINTOOLS_H
#define WINTOOLS_H

#include <QWidget>

/* ���ڱ���������� */
struct TitleBarWidgetItem
{
	TitleBarWidgetItem(QWidget* pWidget, int nAlign = Qt::AlignRight,
		int nSpacing = 0, bool bAutoFillRest = false)
		: widget(pWidget), widgetAlign(nAlign), spacing(nSpacing), autoFillRest(bAutoFillRest)
	{}

	QWidget* widget;
	int		widgetAlign;
	int		spacing = 0;
	bool	autoFillRest;	// �Ƿ��Զ�ռ��ʣ��ռ�,ע��:ֻ����һ���������Ϊtrue
};
typedef QList<TitleBarWidgetItem> TitleBarWidgetList;


#endif	// WINTOOLS_H