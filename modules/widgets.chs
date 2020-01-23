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
    FWidget : Syst
    {
        $ # " Widget base";
        WdgAgent : AVWidget;
        $ # " Allocation";
        AlcX : AStatec;
        AlcY : AStatec;
        AlcW : AStatec;
        AlcW < {
            Debug.Update = y;
            Value = "SI 37";
        }
        AlcH : AStatec;
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
        OutRqsW : /*/Modules/DesComps/StcInpExt;
        OutRqsH : /*/Modules/DesComps/StcInpExt;
        $ # " Connections";
        ./InpAlcX/Int ~ ./AlcX/Inp;
        ./InpAlcY/Int ~ ./AlcY/Inp;
        ./InpAlcW/Int ~ ./AlcW/Inp;
        ./InpAlcH/Int ~ ./AlcH/Inp;
    }
}
