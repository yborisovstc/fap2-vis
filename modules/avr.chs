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
        $ + /ContainerMdl/HLayoutBase;
    }
    UnitDrp : /*/Modules/ContainerMdl/HLayoutBase
    {
        $ # " Unit detail representation";
        CntAgent : AUnitDrp;
        CntAgent < {
            ModelSynced : AStatec;
            ModelSynced < Value = "SB false";
        }
        Padding = 10;
    }
    VrController : Syst
    {
        $ # " Visual representation controller";
        AVrc : AVrController;
        ModelPath : AStatec;
        ModelPath < Value = "SS none";
        ModelCreated : AStatec;
        ModelCreated < Value = "SB false";
        TrsModelCreated : TrModelCreated;
        TrsModelCreated ~ ./ModelCreated/Inp;
        ModelPath ~ ./TrsModelCreated/Inp;
        $ # " Cursor";
        Cursor : AStatec;
        BtnUpInp : StcOutExt;
    }
}
