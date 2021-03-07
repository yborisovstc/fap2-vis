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
    UDrpCp : ASocketMcm
    {
        $ # "Unit DRP output socket";
        InpModelUri : CpStatecInp;
        OutCompsCount : CpStatecOutp;
        OutModelUri : CpStatecOutp;
    }
    UDrpCpp : ASocketMcm
    {
        $ # "Unit DRP output socket";
        InpModelUri : CpStatecOutp;
        OutCompsCount : CpStatecInp;
        OutModelUri : CpStatecInp;
    }
    UDrpCpe : AExtd
    {
        Int : ./../UDrpCp;
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
        RpCp : AExtd
        {
            Int : ./../../UDrpCpp;
        }
        RpCp/Int/OutCompsCount ~ OutCompsCount;
        $ # "Needs to use auxiliary cp to IFR from socket";
        InpModelUri : CpStatecInp;
        RpCp/Int/InpModelUri ~ InpModelUri;
        OutModelUri : CpStatecOutp;
        RpCp/Int/OutModelUri ~ OutModelUri;
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
            MutRmWidget : CpStatecOutp;
            VrvCompsCount : CpStatecInp;
            DrpCreated : CpStatecInp;
            DrpCp : ./../../UDrpCpp;
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
            MutRmWidget : CpStatecInp;
            DrpCreated : CpStatecOutp;
            VrvCompsCount : CpStatecOutp;
            DrpCp : ./../../UDrpCp;
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
        $ # "For debugging only";
        DbgModelUri : AStatec {
            Debug.Update = y;
            Value = "SS nil";
        }
        DbgModelUri/Inp ~ CtrlCp/NavCtrl/DrpCp/OutModelUri;

        $ # " VRP dirty indication";
        VrpDirty : AStatec
        {
            Debug.Update = y;
            Value = "SB false";
        }
        VrpDirty/Inp ~ : ATrcAndVar @ {
            Inp ~ U_Neq : ATrcCmpVar @ {
                Inp ~ : ATrcUri @ {
                    Inp ~ CtrlCp/NavCtrl/DrpCp/OutModelUri;
                };
                Inp2 ~ : ATrcUri @ {
                     Inp ~ Cursor;
                };
            };
            Inp ~ Cmp_Eq : ATrcCmpVar @ {
                Inp ~ CtrlCp/NavCtrl/VrvCompsCount;
                Inp2 ~ Const_1 : AStatec
                {
                    Value = "SI 1";
                };
            };
            Inp ~ C_Neq_2 : ATrcCmpVar @ {
                Inp ~ CtrlCp/NavCtrl/DrpCp/OutModelUri;
                Inp2 ~ Const_SNil;
                C_Neq_2 < Debug.LogLevel = 20;
            };
        };
        $ # "For debugging only";
        VrvCompsCnt : AStatec {
            Debug.Update = y;
            Value = "SI 0";
        }
        $ # " DRP removal on VRP dirty";
        CtrlCp/NavCtrl/MutRmWidget ~ RmWdg : ATrcSwitchBool @ {
            Sel ~ VrpDirty;
            Inp1 ~ : AStatec { Value = "SI -1"; };
            Inp2 ~ : AStatec { Value = "SI 0"; };
            RmWdg < Debug.LogLevel = 20;
        };
        $ # "VrvCompsCnt/Inp ~ CtrlCp/NavCtrl/VrvCompsCount";
        $ # " DRP creation";
        ./CtrlCp/NavCtrl/MutAddWidget ~ Sw1 : ATrcSwitchBool @ {
            Sel ~ DrpCreate_Eq : ATrcCmpVar @ {
                Inp ~ ./CtrlCp/NavCtrl/DrpCreated;
                Inp2 ~ Const_1;
            };
            Inp1 ~ : AStatec { Value = "TPL,SS:name,SS:type,SI:pos Drp /*/Modules/AvrMdl/UnitDrp 0"; };
            Inp2 ~ : AStatec { Value = "TPL,SS:name,SS:type,SI:pos Drp nil 0"; };
            DrpCreate_Eq < Debug.LogLevel = 20;
        };
        Sw1 < Debug.LogLevel = 20;
        $ # " Model set to DRP: needs to connect DRPs input to controller";
        SDrpCreated : AStatec {
            Debug.Update = y;
            Value = "SI 0";
        }
        SDrpCreated/Inp ~ ./CtrlCp/NavCtrl/DrpCreated;
        WindowEdp/InpMut ~ : ATrcSwitchBool @ {
            Sel ~ DrpCreate_Eq;
            Inp1 ~ : AStatec { Value = "MUT none"; };
            Inp2 ~ TMutConn : ATrcMutConn @ {
                Cp1 ~ : AStatec {
                    Value = "SS ./VrvCp/NavCtrl/DrpCp";
                };
                Cp2 ~ : AStatec {
                    Value = "SS /testroot/Test/Window/Scene/VBox/ModelView/Drp/RpCp";
                };
            };
        };
        $ # " Model URI is set only after DRP has been created";
        CtrlCp/NavCtrl/DrpCp/InpModelUri ~ : ATrcSwitchBool @ {
            Sel ~ DelayMdlUri : AStatec @ {
                Inp ~ MdlUriSel : ATrcAndVar @ {
                    Inp ~ : ATrcNegVar @ {
                        Inp ~ Delay : AStatec @ {
                            Inp ~ DrpCreate_Eq;
                            Delay < Value = "SB false";
                            Delay < Debug.Update = y;
                        }; 
                    };
                    Inp ~ DrpCreate_Eq;
                };
                DelayMdlUri < Value = "SB false";
	        DelayMdlUri < Debug.Update = y;
            };
            Inp1 ~ Const_SNil; 
            Inp2 ~ Cursor;
            MdlUriSel < Debug.LogLevel = 20;
        };
    }
}
