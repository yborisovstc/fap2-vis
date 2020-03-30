Pos: 0 -- Wrong context type
ContainerMdl : Elem
{
    About = "FAP2 widget visualization system";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcInpExt;
        $ + /DesComps/StcOutExt;
        $ + /FvWidgets/FWidgetBase;
    }
    FContainer : /*/Modules/FvWidgets/FWidgetBase
    {
        $ # " Container base";
        RqsW < Debug.Update = y;
        RqsH < Debug.Update = y;
        # = " Padding value";
        Padding : AStatec;
        Padding < Value = "SI 10";
        # = " Inputs binded to components requisition outputs";
        RqsWInp : AVCpsCpInp;
        RqsWInp < WcpName = OutRqsW;
        RqsHInp : AVCpsCpInp;
        RqsHInp < WcpName = OutRqsH;
        # = " Outputs binded to components allocation inputs";
        AlcWOut : AVCpsCpOut;
        AlcWOut < WcpName = InpAlcW;
        AlcHOut : AVCpsCpOut;
        AlcHOut < WcpName = InpAlcH;
        AlcXOut : AVCpsCpOut;
        AlcXOut < WcpName = InpAlcX;
        AlcYOut : AVCpsCpOut;
        AlcYOut < WcpName = InpAlcY;
        # = " Set all widgets allocation dependent of requisition";
        RqsWInp ~ AlcXOut;
        RqsWInp ~ AlcYOut;
        RqsWInp ~ AlcWOut;
        RqsWInp ~ AlcHOut;
        RqsHInp ~ AlcXOut;
        RqsHInp ~ AlcYOut;
        RqsHInp ~ AlcWOut;
        RqsHInp ~ AlcHOut;
    }
    HLayoutBase : FContainer
    {
        $ # " Horisontal layout Container base";
        TrRqsW : TrReqSum;
        RqsWInp ~ ./TrRqsW/Inp;
        ./TrRqsW ~ ./RqsW/Inp;
        Padding ~ ./TrRqsW/Padding;
        TrRqsH : TrReqMax;
        RqsHInp ~ ./TrRqsH/Inp;
        Padding ~ ./TrRqsH/Padding;
        ./TrRqsH ~ ./RqsH/Inp;
    }
    HLayout : HLayoutBase
    {
        $ # " Horisontal layout Container";
        CntAgent : AVHLayout;
    }
    VLayout : FContainer
    {
        About = " Vert layout Container";
        CntAgent : AVLayout;
        TrRqsW : TrReqMax;
        RqsWInp ~ ./TrRqsW/Inp;
        Padding ~ ./TrRqsW/Padding;
        ./TrRqsW ~ ./RqsW/Inp;
        TrRqsH : TrReqSum;
        RqsHInp ~ ./TrRqsH/Inp;
        Padding ~ ./TrRqsH/Padding;
        ./TrRqsH ~ ./RqsH/Inp;
    }
    Alignment : FContainer
    {
        About = " Alignment Container";
        CntAgent : AAlignment;
        Slot_1 : AVSlot;
        TrRqsW : TrReqMax;
        RqsWInp ~ ./TrRqsW/Inp;
        Padding ~ ./TrRqsW/Padding;
        ./TrRqsW ~ ./RqsW/Inp;
        TrRqsH : TrReqMax;
        RqsHInp ~ ./TrRqsH/Inp;
        Padding ~ ./TrRqsH/Padding;
        ./TrRqsH ~ ./RqsH/Inp;
    }
}
