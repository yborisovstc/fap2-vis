Pos: 0 -- Wrong context type
testroot : Elem
{
    $ # "UT of Unit DRP";
    Modules : AImports
    {
        $ + /SysComps;
        $ + /IncapsComps;
        $ + /DesComps/Des;
        $ + /DesComps/StcOutExt;
        $ + /DesComps/StcInpExt;
        $ + /GVisComps/GWindow;
        $ + /GVisComps/VisEnv;
        $ + /GVisComps/Scene;
        $ + /GVisComps/SceRect;
        $ + /FvWidgets/FUnitCrp;
        $ + /AvrMdl/UnitDrp;
    }
    Comps : Elem;
    Test : /testroot/Modules/DesComps/Des
    {
        Env : /testroot/Modules/GVisComps/VisEnv
        {
            $ # "Visualisation environment";
            VisEnvAgt < Init = Yes;
            Window : /testroot/Modules/GVisComps/GWindow
            {
                AWnd < Init = Yes;
                Width < Value = "SI 1200";
                Heigth < Value = "SI 800";
                Scene : /testroot/Modules/GVisComps/Scene
                {
                    $ # "Visualisation scene";
                    Drp : /*/Modules/AvrMdl/UnitDrp
                    {
                        Padding = 10;
                        Slot_1 < Wdg1 : /*/Modules/FvWidgets/FUnitCrp
                        {
                            BgColor = "{R:'0.0' G:'0.3' B:'0.0'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                            ./RqsW < {
                                Debug.Update = y;
                                Value = "SI 22";
                            }
                            ./RqsH < {
                                Debug.Update = y;
                                Value = "SI 143";
                            }
                            W1IncrW : ATrcAddVar;
                            W1IncrData : AStatec;
                            W1IncrData < Value = "SI 1";
                            W1IncrData ~ ./W1IncrW/Inp;
                            $ # "./W1IncrW/Out ~ ./RqsW/Inp";
                            $ # "./RqsW ~ ./W1IncrW/Inp";
                        }
                        Slot_2 < Wdg2 : /*/Modules/FvWidgets/FUnitCrp
                        {
                            BgColor = "{R:'0.0' G:'0.0' B:'0.5'}";
                            FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                            ./RqsW < {
                                Debug.Update = y;
                                Value = "SI 22";
                            }
                            ./RqsH < {
                                Debug.Update = y;
                                Value = "SI 143";
                            }
                            W1IncrW : ATrcAddVar;
                            W1IncrData : AStatec;
                            W1IncrData < Value = "SI 1";
                            W1IncrData ~ ./W1IncrW/Inp;
                            $ # "./W1IncrW/Out ~ ./RqsW/Inp";
                            $ # "./RqsW ~ ./W1IncrW/Inp";
                        }
                    }
                }
            }
            EnvWidth : AStatec;
            EnvHeight : AStatec;
            Title : AStatec;
            EnvWidth ~ ./Window/Inp_W;
            EnvHeight ~ ./Window/Inp_H;
            Title ~ ./Window/Inp_Title;
        }
        ./Env/EnvWidth < Value = "SI 640";
        ./Env/EnvHeight < Value = "SI 480";
        ./Env/Title < Value = "SS Title";
    }
}
