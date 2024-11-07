/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <https://www.gnu.org/licenses/>.
 * 
 *  2024-2024 Daniel Gao <daniel.gao.work@gmail.com>
 */

#pragma once

#include "adjuster.h"
#include "guiutils.h"
#include "toolpanel.h"

#include <memory>

class Framing final :
    public ToolParamBlock,
    public AdjusterListener,
    public FoldableToolPanel
{
public:
    static const Glib::ustring TOOL_NAME;

    Framing();
    ~Framing();

    void read(const rtengine::procparams::ProcParams* pp,
              const ParamsEdited* pedited = nullptr) override;
    void write(rtengine::procparams::ProcParams* pp,
               ParamsEdited* pedited = nullptr) override;
    void setDefaults(const rtengine::procparams::ProcParams* defParams,
                     const ParamsEdited* pedited = nullptr) override;
    void setBatchMode(bool batchMode) override;

    void update(bool isCropped, int croppedWidth, int croppedHeight,
                int originalWidth = 0, int originalHeight = 0);

    // AdjusterListener
    void adjusterChanged(Adjuster* adj, double newVal) override;

    // Signal connections
    void onFramingMethodChanged();
    void onAspectRatioChanged();
    void onOrientationChanged();
    void onWidthChanged();
    void onHeightChanged();
    void onAllowUpscalingToggled();
    void onBorderSizeMethodChanged();
    void onBasisChanged();
    void onMinSizeToggled();
    void onMinWidthChanged();
    void onMinHeightChanged();
    void onAbsWidthChanged();
    void onAbsHeightChanged();

private:
    class AspectRatios;

    // Helper struct for repeated patterns
    struct DimensionGui
    {
        using CallbackFunc = void(Framing::*)();

        DimensionGui() = default;
        DimensionGui(Gtk::Box* parent, const char* text);

        void setValue(int newValue) { value->set_value(newValue); }
        void setRange(int min, int max) { value->set_range(min, max); }
        void connect(Framing& framing, CallbackFunc callback);

        void show() { box->show(); }
        void hide() { box->hide(); }

        Gtk::Box* box;
        MySpinButton* value;
        sigc::connection connection;
        bool isDirty;
    };

    void setupFramingMethodGui();
    void setupBorderSizeGui();
    void setupBorderColorsGui();

    void readParams(const rtengine::procparams::ProcParams* pp);
    void readEdited(const ParamsEdited* pedited);
    void writeParams(rtengine::procparams::ProcParams* pp);
    void writeEdited(ParamsEdited* pedited);

    void setDimensions();
    void updateFramingMethodGui();
    void updateBorderSizeGui();

    // Framing method
    MyComboBoxText* framingMethod;
    sigc::connection framingMethodChanged;
    Gtk::Label* aspectRatioLabel;
    MyComboBoxText* aspectRatio;
    sigc::connection aspectRatioChanged;
    Gtk::Label* orientationLabel;
    MyComboBoxText* orientation;
    sigc::connection orientationChanged;
    DimensionGui width;
    DimensionGui height;
    Gtk::CheckButton* allowUpscaling;
    sigc::connection allowUpscalingConnection;

    // Border sizing
    MyComboBoxText* borderSizeMethod;
    sigc::connection borderSizeMethodChanged;
    Gtk::Label* basisLabel;
    MyComboBoxText* basis;
    sigc::connection basisChanged;
    Adjuster* relativeBorderSize;
    Gtk::Frame* minSizeFrame;
    Gtk::Box* minSizeFrameContent;
    Gtk::CheckButton* minSizeEnabled;
    sigc::connection minSizeEnabledConnection;
    DimensionGui minWidth;
    DimensionGui minHeight;
    DimensionGui absWidth;
    DimensionGui absHeight;

    // Border colors
    Adjuster* redAdj;
    Adjuster* greenAdj;
    Adjuster* blueAdj;

    IdleRegister idleRegister;
    std::unique_ptr<AspectRatios> aspectRatioData;

    int imgWidth;
    int imgHeight;
};