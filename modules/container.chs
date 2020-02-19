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
        CntAgent : AVHLayout
        {
            RqsWInp : AVCpsCp;
            RqsWInp < WcpName = OutRqsW;
            AlcWOut : AVCpsCp;
            AlcWOut < WcpName = InpAlcW;
        }
        Slot_1 : AVSlot;
        Slot_2 : AVSlot;
    }
}
