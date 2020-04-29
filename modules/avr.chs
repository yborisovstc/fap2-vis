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
    VrController : Syst
    {
        $ # " Visual representation controller";
        AVrc : AVrController;
        ModelPath : AStatec;
        ModelPath < Value = "SS none";
        ModelCreated : AStatec;
        ModelCreated < Debug.Update = y;
        ModelCreated < Value = "SB false";
        TrsModelCreated : TrModelCreated;
        TrsModelCreated ~ ./ModelCreated/Inp;
        ModelPath ~ ./TrsModelCreated/Inp;
        $ # " Cursor";
        Cursor : AStatec;
        Cursor < Debug.Update = y;
        Cursor < Value = "SS nil";
        CtrlCp : ./../VrControllerCp;
        TrCur : TrCursor;
        ./Cursor/Inp ~ TrCur;
        ./TrCur/InpCmdUp ~ ./CtrlCp/NavCtrl/CmdUp;
        ./TrCur/InpNodeSelected ~ ./CtrlCp/NavCtrl/NodeSelected;
        ./TrCur/InpCursor ~ Cursor;
        ./TrCur/InpMdlCreated ~ ModelCreated;
        CursorApplied : AStatec;
        CursorApplied < Debug.Update = y;
        CursorApplied < Value = "SS nil";
        TrCurApplied : TrCursorApplied;
        ./CursorApplied/Inp ~ TrCurApplied;
        ./TrCurApplied/Inp ~ Cursor;
    }
}
