FvWidgetsL : Elem
{
    About = "FAP2 visualization system. Widget-to-slot linkage approach";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcInpExt;
        $ + /DesComps/StcOutExt;
    }
    WidgetCp : ASocketMcm
    {
        InpAlcX : CpStatecOutp;
        InpAlcY : CpStatecOutp;
        InpAlcW : CpStatecOutp;
        InpAlcH : CpStatecOutp;
        OutAlcX : CpStatecInp;
        OutAlcY : CpStatecInp;
        OutAlcW : CpStatecInp;
        OutAlcH : CpStatecInp;
        RqsW : CpStatecInp;
        RqsH : CpStatecInp;
    }
    FWidgetBase : Syst
    {
        $ # " Widget base";
        FontPath = /usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf;
        Cp : ./../WidgetCp;
        $ # " Allocation";
        AlcX : AStatec;
        AlcX < {
            Debug.Update = y;
            Value = "SI 0";
        }
        AlcY : AStatec;
        AlcY < {
            Debug.Update = y;
            Value = "SI 0";
        }
        AlcW : AStatec;
        AlcW < {
            Debug.Update = y;
            Value = "SI 0";
        }
        AlcH : AStatec;
        AlcH < {
            Debug.Update = y;
            Value = "SI 0";
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
        $ # " Connections";
        ./AlcX/Inp ~ ./Cp/InpAlcX;
        ./AlcY/Inp ~ ./Cp/InpAlcY;
        ./AlcW/Inp ~ ./Cp/InpAlcW;
        ./AlcH/Inp ~ ./Cp/InpAlcH;
        AlcX ~ ./Cp/OutAlcX;
        AlcY ~ ./Cp/OutAlcY;
        AlcW ~ ./Cp/OutAlcW;
        AlcH ~ ./Cp/OutAlcH;
        RqsW ~ ./Cp/RqsW;
        RqsH ~ ./Cp/RqsH;
    }
    FLabelL : FWidgetBase
    {
        $ # " Label";
        WdgAgent : AVLabel;
    }
    FButtonL : FWidgetBase
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
