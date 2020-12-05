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
        InpModelUri : CpStatecInp;
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
        InpModelUri : CpStatecInp;
    }
    VrViewCp : ASocketMcm
    {
        About = "Vis representation view CP";
        NavCtrl : ASocketMcm
        {
            About = "Navigation control";
            CmdUp : CpStatecInp;
            NodeSelected : CpStatecInp;
            MutAddWidget : CpStatecOutp;
            DrpCreated : CpStatecInp;
            ModelUri : CpStatecOutp;
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
            MutAddWidget : CpStatecInp;
            DrpCreated : CpStatecOutp;
            ModelUri : CpStatecInp;
        }
    }
    VrController : /*/Modules/DesComps/Des
    {
        $ # " Visual representation controller";
        $ # " Model adapter. Set AgentUri content to model URI.";
        CursorUdp : /*/Modules/AdpComps/UnitAdp;
        $ # " Model view adapter. Set AgentView cnt to model view.";
        ModelViewUdp : /*/Modules/AdpComps/UnitAdp;
        $ # " Window MElem adapter";
        WindowEdp : /*/Modules/AdpComps/MelemAdp;
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
        $ # "!! Const_SMdlRoot < Value = SS ./../../ModelMnt/*";
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
        $ # " VRP control test";
        TestDrp : AStatec;
        TestDrp < Debug.Update = y;
        TestDrp < Value = "TPL,SS:name,SS:type,SI:pos Drp /*/Modules/AvrMdl/UnitDrp 0";
        ./CtrlCp/NavCtrl/MutAddWidget ~ TestDrp;
        $ # " Model set to DRP: needs to connect DRPs input to controller";
        TMutConn : ATrcMutConn;
        MutConnCp1 : AStatec;
        MutConnCp1 < Value = "SS ./VrvCp/NavCtrl/ModelUri";
        MutConnCp2 : AStatec;
        MutConnCp2 < Value = "SS /testroot/Test/Window/Scene/VBox/ModelView/Drp/InpModelUri";
        TMutConn/Cp1 ~ MutConnCp1;
        TMutConn/Cp2 ~ MutConnCp2;
        TSwitch : ATrcSwitchBool;
        TCmp_Eq : ATrcCmpVar;
        SDrpCreated : AStatec;
        SDrpCreated < Debug.Update = y;
        SDrpCreated < Value = "SI 0";
        SDrpCreated/Inp ~ ./CtrlCp/NavCtrl/DrpCreated;
        TCmp_Eq/Inp ~ ./CtrlCp/NavCtrl/DrpCreated;
        TCmp_Eq/Inp2 ~ Const_1;
        TSwitch/Sel ~ TCmp_Eq;
        Const_MutNone : AStatec;
        Const_MutNone < Value = "MUT none";
        TSwitch/Inp1 ~ Const_MutNone;
        TSwitch/Inp2 ~ TMutConn;
        WindowEdp/InpMut ~ TSwitch;
        ./CtrlCp/NavCtrl/ModelUri ~ Cursor;
    }
}
