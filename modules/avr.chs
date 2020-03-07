AvrMdl : Elem
{
    About = "Agents visual representations";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcInpExt;
        $ + /DesComps/StcOutExt;
        $ + /FvWidgets/FWidgetBase;
        $ + /ContainerMdl/FContainer;
    }
    UnitDrp : /*/Modules/ContainerMdl/FContainer
    {
        $ # " Unit detail representation";
        CntAgent : AUnitDrp;
        CntAgent < {
            RqsWInp : AVCpsCp;
            RqsWInp < WcpName = OutRqsW;
            AlcWOut : AVCpsCp;
            AlcWOut < WcpName = InpAlcW;
            AlcHOut : AVCpsCp;
            AlcHOut < WcpName = InpAlcH;
            AlcXOut : AVCpsCp;
            AlcXOut < WcpName = InpAlcX;
            AlcYOut : AVCpsCp;
            AlcYOut < WcpName = InpAlcY;
            ModelSynced : AStatec;
            ModelSynced < Value = "SB false";
        }
        Slot_1 : AVSlot;
        Slot_2 : AVSlot;
    }
    VrController : Syst
    {
        $ # " Visual representation controller";
        AVrc : AVrController;
        ModelPath : AStatec;
        ModelCreated : AStatec;
        ModelCreated < Value = "SB false";
        TrsModelCreated : TrModelCreated;
        ./TrsModelCreated/Out ~ ./ModelCreated/Inp;
        ModelPath ~ ./TrsModelCreated/Inp;
    }
}
