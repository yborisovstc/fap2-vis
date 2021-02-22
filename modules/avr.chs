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
        CntAgent : AUnitDrp {
            ModelSynced : AStatec {
                Value = "SB false";
            }
        }
        Padding = 10;
        InpModelUri : CpStatecInp;
        OutModelUri : CpStatecOutp;
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
        CursorUdp/InpMagUri ~ Cursor : AStatec {
            Debug.Update = y;
            Value = "SS nil";
        };
        Const_SNil : AStatec {
            Value = "SS nil";
        }
        Cursor/Inp ~ : ATrcSwitchBool @ {
            Sel ~ Cmp_Eq_2 : ATrcCmpVar @ {
	        Inp ~ Cursor;
                Inp2 ~ Const_SNil;
            };
	    Inp1 ~ : ATrcSwitchBool @ {
                Inp1 ~ ./CtrlCp/NavCtrl/NodeSelected;
                Inp2 ~ Cursor;
                Sel ~ Cmp_Eq_3 : ATrcCmpVar @ {
                    Inp ~ Const_SNil;
                    Inp2 ~ ./CtrlCp/NavCtrl/NodeSelected;
                };
            };
            Inp2 ~ Const_SMdlRoot : AStatec {
                $ # "!! Value = SS ./../../ModelMnt/*";
            };
        };
        $ # " VRP dirty indication";
        VrpDirty : AStatec
        {
            Debug.Update = y;
            Value = "SB false";
        }
        VrpDirty/Inp ~ : ATrcAndVar @ {
            Inp ~ : AStatec
            {
                Value = "SB true";
            };
            Inp ~ Cmp_Eq : ATrcCmpVar @ {
                Inp ~ ModelViewUdp/CompsCount;
                Inp2 ~ Const_1 : AStatec
                {
                    Value = "SI 1";
                };
            };
        };
        $ # " VRP control test";
        ./CtrlCp/NavCtrl/MutAddWidget ~ TestDrp : AStatec
        {
            Debug.Update = y;
            Value = "TPL,SS:name,SS:type,SI:pos Drp /*/Modules/AvrMdl/UnitDrp 0";
        };
        $ # " Model set to DRP: needs to connect DRPs input to controller";
        SDrpCreated : AStatec {
            Debug.Update = y;
            Value = "SI 0";
        }
        SDrpCreated/Inp ~ ./CtrlCp/NavCtrl/DrpCreated;
        WindowEdp/InpMut ~ : ATrcSwitchBool @ {
            Sel ~ TCmp_Eq : ATrcCmpVar @ {
                Inp ~ ./CtrlCp/NavCtrl/DrpCreated;
                Inp2 ~ Const_1;
            };
            Inp1 ~ : AStatec {
                Value = "MUT none";
            };
            Inp2 ~ TMutConn : ATrcMutConn @ {
                Cp1 ~ : AStatec {
                    Value = "SS ./VrvCp/NavCtrl/ModelUri";
                };
                Cp2 ~ : AStatec {
                    Value = "SS /testroot/Test/Window/Scene/VBox/ModelView/Drp/InpModelUri";
                };
            };
        };
        ./CtrlCp/NavCtrl/ModelUri ~ Cursor;
    }
}
