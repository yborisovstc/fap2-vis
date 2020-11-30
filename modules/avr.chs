AvrMdl : Elem
{
    About = "Agents visual representations";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcInpExt;
        $ + /DesComps/StcOutExt;
        $ + /AdpComps/UnitAdp;
        $ + /AdpComps/MelemAdp;
        $ + /FvWidgets/FWidgetBase;
        $ + /ContainerModL/FHLayoutLBase;
    }
    UnitDrp : /*/Modules/ContainerModL/FHLayoutLBase
    {
        $ # " Unit detail representation";
        CntAgent : AUnitDrp;
        CntAgent < {
            ModelSynced : AStatec;
            ModelSynced < Value = "SB false";
        }
        Padding = 10;
    }
    SystDrp : /*/Modules/ContainerModL/FHLayoutLBase
    {
        $ # " Syst detail representation";
        CntAgent : ASystDrp;
        CntAgent < {
            ModelSynced : AStatec;
            ModelSynced < Value = "SB false";
        }
        Padding = 10;
    }
    VrViewCp : ASocketMcm
    {
        About = "Vis representation view CP";
        NavCtrl : ASocketMcm
        {
            About = "Navigation control";
            CmdUp : CpStatecInp;
            NodeSelected : CpStatecInp;
        }
    }
    VrControllerCp : ASocketMcm
    {
        About = "Vis representation controller CP";
        NavCtrl : ASocketMcm
        {
            About = "Navigation control";
            CmdUp : CpStatecOutp;
            NodeSelected : CpStatecOutp;
        }
    }
    VrController : /*/Modules/DesComps/Des
    {
        $ # " Visual representation controller";
        $ # " Model adapter. Set AgentUri content to model URI.";
        CursorUdp : /*/Modules/AdpComps/UnitAdp;
        $ # " Model view adapter. Set AgentView cnt to model view.";
        ModelViewUdp : /*/Modules/AdpComps/UnitAdp;
        ModelViewEdp : /*/Modules/AdpComps/MelemAdp;
        $ # "Model mounting";
        ModelMnt : AMntp;
        ModelMnt < EnvVar = Model;
        $ # "Model adapter";
        ModelUdp : /*/Modules/AdpComps/UnitAdp;
        ModelUdp/AdpAgent < AgentUri = ./../../ModelMnt;
        $ # "CP binding to view";
        CtrlCp : ./../VrControllerCp;
        $ # " Cursor";
        Cursor : AStatec;
        Cursor < Debug.Update = y;
        Cursor < Value = "SS nil";
        CursorUdp/InpMagUri ~ Cursor;
        Sw1 : ATrcSwitchBool;
        Cursor/Inp ~ Sw1;
        Cmp_Eq_2 : ATrcCmpVar;
        Cmp_Eq_2/Inp ~ Cursor;
        Const_SNil : AStatec;
        Const_SNil < Value = "SS nil";
        Cmp_Eq_2/Inp2 ~ Const_SNil;
        Sw1/Sel ~ Cmp_Eq_2;
        Const_SMdlRoot : AStatec;
        Const_SMdlRoot < Value = "SS ./../../ModelMnt/*";
        Sw1/Inp2 ~ Const_SMdlRoot;
        Sw2 : ATrcSwitchBool;
        Sw1/Inp1 ~ Sw2;
        Sw2/Inp1 ~ ./CtrlCp/NavCtrl/NodeSelected;
        Sw2/Inp2 ~ Cursor;
        Cmp_Eq_3 : ATrcCmpVar;
        Cmp_Eq_3/Inp ~ Const_SNil;
        Cmp_Eq_3/Inp2 ~ ./CtrlCp/NavCtrl/NodeSelected;
        Sw2/Sel ~ Cmp_Eq_3;
        $ # " VRP dirty indication";
        VrpDirty : AStatec;
        VrpDirty < Debug.Update = y;
        VrpDirty < Value = "SB false";
        And_1 : ATrcAndVar;
        VrpDirty/Inp ~ And_1;
        Const_1 : AStatec;
        Const_1 < Value = "SI 1";
        Const_T : AStatec;
        Const_T < Value = "SB true";
        And_1/Inp ~ Const_T;
        Cmp_Eq : ATrcCmpVar;
        Cmp_Eq/Inp ~ ModelViewUdp/CompsCount;
        Cmp_Eq/Inp2 ~ Const_1;
        And_1/Inp ~ Cmp_Eq;
    }
}
