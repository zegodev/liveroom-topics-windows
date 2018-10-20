#include "stdafx.h"
#include "ZGSlider.h"
#include "AppSupport/ZGLog.h"

ZGSlider::ZGSlider()
{
}

ZGSlider::~ZGSlider()
{
}

void ZGSlider::OnSliderPosChange(std::function<void(int pos)> cb)
{
    pos_change_notify_ = cb;
}

BEGIN_MESSAGE_MAP(ZGSlider, CSliderCtrl)
    //{{AFX_MSG_MAP(CBitmapSlider)
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void ZGSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
    CSliderCtrl::OnLButtonDown(nFlags, point);
    CRect   rectClient, rectChannel;
    GetClientRect(rectClient);
    GetChannelRect(rectChannel);
    int nMax = 0;
    int nMin = 0;
    GetRange(nMin, nMax);
    int nPos = (nMax - nMin)*(point.x - rectClient.left - rectChannel.left) / (rectChannel.Width() - 5);
    SetPos(nPos);
    //ZGLog("slider new pos : %d", nPos);
    if (pos_change_notify_)
    {
        pos_change_notify_(nPos);
    }

}