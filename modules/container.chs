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
    }
    HLayout : FContainer
    {
        $ # " Horisontal layout Container";
        CntAgent : AVHLayout;
        CntAgent < {
            RqsWInp : AVCpsCp;
            RqsWInp < WcpName = OutRqsW;
            RqsHInp : AVCpsCp;
            RqsHInp < WcpName = OutRqsH;
            AlcWOut : AVCpsCp;
            AlcWOut < WcpName = InpAlcW;
            AlcHOut : AVCpsCp;
            AlcHOut < WcpName = InpAlcH;
            AlcXOut : AVCpsCp;
            AlcXOut < WcpName = InpAlcX;
            AlcYOut : AVCpsCp;
            AlcYOut < WcpName = InpAlcY;
        }
    }
    VLayout : FContainer
    {
        About = " Vert layout Container";
        CntAgent : AVLayout;
        CntAgent < {
            RqsWInp : AVCpsCp;
            RqsWInp < WcpName = OutRqsW;
            RqsHInp : AVCpsCp;
            RqsHInp < WcpName = OutRqsH;
            AlcWOut : AVCpsCp;
            AlcWOut < WcpName = InpAlcW;
            AlcHOut : AVCpsCp;
            AlcHOut < WcpName = InpAlcH;
            AlcXOut : AVCpsCp;
            AlcXOut < WcpName = InpAlcX;
            AlcYOut : AVCpsCp;
            AlcYOut < WcpName = InpAlcY;
        }
    }
    Alignment : FContainer
    {
        About = " Alignment Container";
        CntAgent : AAlignment;
        CntAgent < {
            RqsWInp : AVCpsCp;
            RqsWInp < WcpName = OutRqsW;
            RqsHInp : AVCpsCp;
            RqsHInp < WcpName = OutRqsH;
            AlcWOut : AVCpsCp;
            AlcWOut < WcpName = InpAlcW;
            AlcHOut : AVCpsCp;
            AlcHOut < WcpName = InpAlcH;
            AlcXOut : AVCpsCp;
            AlcXOut < WcpName = InpAlcX;
            AlcYOut : AVCpsCp;
            AlcYOut < WcpName = InpAlcY;
        }
        Slot_1 : AVSlot;
    }
}
