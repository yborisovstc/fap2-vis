Pos: 0 -- Wrong context type
FvWidgets : Elem
{
    About = "FAP2 widget visualization system";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcInpExt;
        $ + /DesComps/StcOutExt;
    }
    FWidgetBase : Syst
    {
        $ # " Widget base";
	FontPath = /usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf;
        $ # " Allocation";
        AlcX : AStatec;
        AlcX < {
            Debug.Update = y;
            Value = "SI 10";
        }
        AlcY : AStatec;
        AlcY < {
            Debug.Update = y;
            Value = "SI 10";
        }
        AlcW : AStatec;
        AlcW < {
            Debug.Update = y;
            Value = "SI 37";
        }
        AlcH : AStatec;
        AlcH < {
            Debug.Update = y;
            Value = "SI 60";
        }
        $ # " Requisition";
        RqsW : AStatec;
        RqsW < {
            Debug.Update = y;
            Value = "SI 0";
        }
        RqsH : AStatec;
        RqsH < {
            Debug.Update = y;
            Value = "SI 0";
        }
        $ # " Inputs";
        $ # " Inputs: Allocation";
        InpAlcX : /*/Modules/DesComps/StcInpExt;
        InpAlcY : /*/Modules/DesComps/StcInpExt;
        InpAlcW : /*/Modules/DesComps/StcInpExt;
        InpAlcH : /*/Modules/DesComps/StcInpExt;
        $ # " Outputs";
        OutRqsW : /*/Modules/DesComps/StcOutExt;
        OutRqsH : /*/Modules/DesComps/StcOutExt;
        $ # " Connections";
        ./InpAlcX/Int ~ ./AlcX/Inp;
        ./InpAlcY/Int ~ ./AlcY/Inp;
        ./InpAlcW/Int ~ ./AlcW/Inp;
        ./InpAlcH/Int ~ ./AlcH/Inp;
        RqsW ~ ./OutRqsW/Int;
        RqsH ~ ./OutRqsH/Int;
    }
    FWidget : FWidgetBase
    {
        $ # " Widget";
        WdgAgent : AVWidget;
    }
    FLabel : FWidgetBase
    {
        $ # " Label";
        WdgAgent : AVLabel;
    }
    FButton : FWidgetBase
    {
        $ # " Button";
        WdgAgent : AButton;
        Pressed : AStatec;
        Pressed < Debug.Update = y;
        Pressed < Value = "SB false";
        PressedReset : AStatec;
        PressedReset < Value = "SB false";
        ./Pressed/Inp ~ PressedReset;
    }
    FUnitCrp : FWidgetBase
    {
        $ # " Unit visual repesentation";
        WdgAgent : AUnitCrp;
        BgColor = "{R:'0.0' G:'0.3' B:'0.0'}";
        FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
    }
}
