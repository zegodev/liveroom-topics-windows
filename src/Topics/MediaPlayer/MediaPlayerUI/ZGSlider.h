//
// ZGSlider.h
//
// Copyright 2018Äê Zego. All rights reserved.
// 

#ifndef ZGSlider_h__
#define ZGSlider_h__

#include <functional>

class ZGSlider : public CSliderCtrl
{
public:
    ZGSlider();
    ~ZGSlider();

    void OnSliderPosChange(std::function<void(int pos)> cb);

protected:

    void OnLButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    std::function<void(int pos)> pos_change_notify_;
};


#endif // ZGSlider_h__

