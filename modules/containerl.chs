ContainerModL : Elem
{
    About = "FAP2 widget visualization system";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcInpExt;
        $ + /DesComps/StcOutExt;
        $ + /FvWidgetsL/FWidgetBase;
    }
    SlotCp : ASocketMcm
    {
        InpAlcX : CpStatecInp;
        InpAlcY : CpStatecInp;
        InpAlcW : CpStatecInp;
        InpAlcH : CpStatecInp;
        OutAlcX : CpStatecOutp;
        OutAlcY : CpStatecOutp;
        OutAlcW : CpStatecOutp;
        OutAlcH : CpStatecOutp;
        RqsW : CpStatecOutp;
        RqsH : CpStatecOutp;
    }
    FContainerL : /*/Modules/FvWidgetsL/FWidgetBase
    {
        $ # " Container base";
        RqsW < Debug.Update = y;
        RqsH < Debug.Update = y;
        # = " Padding value";
        Padding : AStatec;
        Padding < Value = "SI 10";
        InpMutAddWidget : CpStatecInp;
        OutCompsCount : CpStatecOutp;
        OutCompNames : CpStatecOutp;
    }
    FSlotl : AVSlotL
    {
        SCp : ./../SlotCp;
    }
    $ # " Linear layout slot";
    SlotLinNextCp : ASocketMcm
    {
        AlcX : CpStatecOutp;
        AlcY : CpStatecOutp;
        AlcW : CpStatecOutp;
        AlcH : CpStatecOutp;
        CntRqsW : CpStatecOutp;
        CntRqsH : CpStatecOutp;
        Padding : CpStatecOutp;
    }
    SlotLinPrevCp : ASocketMcm
    {
        AlcX : CpStatecInp;
        AlcY : CpStatecInp;
        AlcW : CpStatecInp;
        AlcH : CpStatecInp;
        CntRqsW : CpStatecInp;
        CntRqsH : CpStatecInp;
        Padding : CpStatecInp;
    }
    FSlotlLin : FSlotl
    {
        Prev : ./../SlotLinPrevCp;
        Next : ./../SlotLinNextCp;
        Prev/Padding ~ Next/Padding;
    }
    FVLayoutLSlot : FSlotlLin
    {
        $ # " Vertical layout slot";
        Prev/AlcX ~ SCp/OutAlcX;
        Prev/AlcY ~ SCp/OutAlcY;
        Prev/AlcW ~ SCp/OutAlcW;
        Prev/AlcH ~ SCp/OutAlcH;
        SCp/InpAlcW ~ SCp/RqsW;
        SCp/InpAlcH ~ SCp/RqsH;
        SCp/InpAlcX ~ Next/AlcX;
        Add1 : ATrcAddVar;
        SCp/InpAlcY ~ Add1;
        Add1/Inp ~ Next/AlcY;
        Add1/Inp ~ Next/Padding;
        Add1/Inp ~ Next/AlcH;
        Max1 : ATrcMaxVar;
        Prev/CntRqsW ~ Max1;
        Max1/Inp ~ Next/CntRqsW;
        Max1/Inp ~ SCp/RqsW;
    }
    FLinearLayoutL : FContainerL
    {
        Start : ./../SlotLinPrevCp;
        ./Start/Padding ~ Padding;
        End : ./../SlotLinNextCp;
    }
    FVLayoutL : FLinearLayoutL
    {
        CntAgent : AVLayoutL;
        Add2 : ATrcAddVar;
        ./RqsW/Inp ~ End/CntRqsW;
        ./RqsH/Inp ~ Add2;
        Add2/Inp ~ End/AlcY;
        Add2/Inp ~ End/AlcH;
        Add2/Inp ~ End/Padding;
    }
    FHLayoutLSlot : FSlotlLin
    {
        $ # " Horisontal layout slot";
        Prev/AlcX ~ SCp/OutAlcX;
        Prev/AlcY ~ SCp/OutAlcY;
        Prev/AlcW ~ SCp/OutAlcW;
        Prev/AlcH ~ SCp/OutAlcH;
        SCp/InpAlcW ~ SCp/RqsW;
        SCp/InpAlcH ~ SCp/RqsH;
        SCp/InpAlcY ~ Next/AlcY;
        Add1 : ATrcAddVar;
        SCp/InpAlcX ~ Add1;
        Add1/Inp ~ Next/AlcX;
        Add1/Inp ~ Next/Padding;
        Add1/Inp ~ Next/AlcW;
        Max1 : ATrcMaxVar;
        Prev/CntRqsH ~ Max1;
        Max1/Inp ~ Next/CntRqsH;
        Max1/Inp ~ SCp/RqsH;
    }
    FHLayoutLBase : FLinearLayoutL
    {
        Add2 : ATrcAddVar;
        ./RqsW/Inp ~ Add2;
        Add2/Inp ~ End/AlcX;
        Add2/Inp ~ End/AlcW;
        Add2/Inp ~ End/Padding;
        ./RqsH/Inp ~ End/CntRqsH;
    }
    FHLayoutL : FHLayoutLBase
    {
        CntAgent : AVLayoutL;
    }
    AlignmentSlot : FSlotlLin
    {
        $ # " Horisontal layout slot";
        Prev/AlcX ~ SCp/OutAlcX;
        Prev/AlcY ~ SCp/OutAlcY;
        Prev/AlcW ~ SCp/OutAlcW;
        Prev/AlcH ~ SCp/OutAlcH;
        SCp/InpAlcW ~ SCp/RqsW;
        SCp/InpAlcH ~ SCp/RqsH;
        AddX : ATrcAddVar;
        SCp/InpAlcX ~ AddX;
        AddX/Inp ~ Next/AlcX;
        AddX/Inp ~ Next/Padding;
        AddY : ATrcAddVar;
        SCp/InpAlcY ~ AddY;
        AddY/Inp ~ Next/AlcY;
        AddY/Inp ~ Next/Padding;
        AddCW : ATrcAddVar;
        Prev/CntRqsW ~ AddCW;
        AddCW/Inp ~ SCp/RqsW;
        AddCW/Inp ~ Next/Padding;
        AddCH : ATrcAddVar;
        Prev/CntRqsH ~ AddCH;
        AddCH/Inp ~ SCp/RqsH;
        AddCH/Inp ~ Next/Padding;
    }
    Alignment : FLinearLayoutL
    {
        CntAgent : AAlignment;
        Slot : ./../AlignmentSlot;
        ./Slot/Next ~ ./Start;
        ./End ~ ./Slot/Prev;
        ./RqsW/Inp ~ ./End/CntRqsW;
        ./RqsH/Inp ~ ./End/CntRqsH;
    }
}
