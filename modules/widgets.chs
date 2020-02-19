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
        RqsH : AStatec;
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
}
