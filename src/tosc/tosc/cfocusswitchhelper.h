#ifndef CFOCUSSWITCHHELPER_H
#define CFOCUSSWITCHHELPER_H

//#include "kcontext.h"
#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QComboBox>

template <typename T>
class CFocusSwitchHelper : QObject
{
public:
    /* 创建事件过滤器 */
    CFocusSwitchHelper(T* pWindow): m_pWindow(pWindow)
    {
		m_bReturnInsteadTabKey = true; // gContext->returnInsteadTabKey();
		m_bUpDownInsteadTabKey = true; // gContext->upDownInsteadTabKey();
    }

    ~CFocusSwitchHelper()
    {
        m_oWidgetList.clear();
    }

    /* 添加过滤组件 */
    void addFocusWidget(QWidget* widget)
    {
        if (!m_oWidgetList.contains(widget))
        {
            m_oWidgetList.append(widget);
            widget->installEventFilter(this);
        }
    }

    /* 移除过滤组件 */
    void removeFocusWidget(QWidget* widget)
    {
        m_oWidgetList.removeOne(widget);
    }

    /* 上下方向键切换焦点生效 */
	void setUpDownInsteadTabKeyValid(bool bValid)
    {
		m_bUpDownInsteadTabKey = bValid;
    }

    /* 回车向下切换焦点生效 */
	void setReturnInsteadTabKeyValid(bool bValid)
    {
		m_bReturnInsteadTabKey = bValid;
    }
	
protected:
    /* 过滤键盘事件 */
    bool eventFilter(QObject* obj, QEvent * evt)
    {
        // 检查事件
        if (evt->type() != QEvent::KeyPress) return false;
        QKeyEvent* keyPress = reinterpret_cast<QKeyEvent*>(evt);

		if ((m_bReturnInsteadTabKey && (keyPress->key() == Qt::Key_Return || 
			                            keyPress->key() == Qt::Key_Enter)) ||
			(m_bUpDownInsteadTabKey && (keyPress->key() == Qt::Key_Down ||
										keyPress->key() == Qt::Key_Up ||
										keyPress->key() == Qt::Key_Left ||
										keyPress->key() == Qt::Key_Right)))
        {
            // 检查组件
            QWidget* widget = qobject_cast<QWidget*>(obj);
            if (!widget) return false;
            if (!m_oWidgetList.contains(widget)) return false;

            int nKey = keyPress->key();

            // 向后切换焦点
			if (nKey == Qt::Key_Down)
            {
                return m_pWindow->focusNextChild();
            }

			// 如果是按钮则触发默认按钮默认操作
			if (nKey == Qt::Key_Return || nKey == Qt::Key_Enter)
			{
				if (widget->inherits("QPushButton"))
				{
					return false;
				}
				return m_pWindow->focusNextChild();
			}

            // 向前切换焦点
            if (nKey == Qt::Key_Up)
            {
                return m_pWindow->focusPreviousChild();
            }

            // 下拉组合框用左右方向键代替上下方向键
			if (widget->inherits("QComboBox") && !widget->inherits("CCheckComboBox"))
            {
                QComboBox* combo = qobject_cast<QComboBox*>(widget);
				if (combo->count() == 0) return false;

                if (nKey == Qt::Key_Left)
                {
                    int nIndex = combo->currentIndex() - 1;
                    if (nIndex >= 0)
                    {
                        combo->setCurrentIndex(nIndex);
                    }
					else
					{
						combo->setCurrentIndex(combo->count() - 1);
					}
					return true;
                }
                else if (nKey == Qt::Key_Right)
                {
                    int nIndex = combo->currentIndex() + 1;
                    if (nIndex < combo->count())
                    {
                        combo->setCurrentIndex(nIndex);
                    }
					else
					{
						combo->setCurrentIndex(0);
					}
					return true;
                }
            }
        }
        return false;
    }

private:
	bool m_bReturnInsteadTabKey;		// 回车键代替Tab键
	bool m_bUpDownInsteadTabKey;		// 上下方向键键代替Tab键

    T* m_pWindow;
    QList<QWidget*> m_oWidgetList;
};

#endif // CFOCUSSWITCHHELPER_H
