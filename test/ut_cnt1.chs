testroot : Elem
{
    $ # "Unit test of Container base agent";
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
        $ + /FvWidgets/FWidget;
        $ + /ContainerMdl/HLayout;
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
                    Cnt1 : /*/Modules/ContainerMdl/HLayout
                    {
                        Padding = 20;
                        AlcW < Value = "SI 220";
                        AlcH < Value = "SI 330";
                        Slot_1 : AVSlot;
                        Slot_1 < Wdg1 : /*/Modules/FvWidgets/FWidget
                        {
                            BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
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
                            ./W1IncrW/Out ~ ./RqsW/Inp;
                            ./RqsW ~ ./W1IncrW/Inp;
                        }
                        Slot_2 : AVSlot;
                        Slot_2 < Wdg2 : /*/Modules/FvWidgets/FWidget
                        {
                            BgColor = "{R:'1.0' G:'0.0' B:'0.0'}";
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
                            ./W1IncrW/Out ~ ./RqsW/Inp;
                            ./RqsW ~ ./W1IncrW/Inp;
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
    $ # " Increasing size of widget";
    WdgWidth : AStatec;
    WdgWidth < {
        Debug.Update = y;
        Value = "SI 40";
    }
    WdgHeight : AStatec;
    WdgHeight < {
        Debug.Update = y;
        Value = "SI 60";
    }
    IncrW : ATrcAddVar;
    IncrH : ATrcAddVar;
    IncrData : AStatec;
    IncrData < Value = "SI 1";
    IncrData ~ ./IncrW/Inp;
    ./IncrW/Out ~ ./WdgWidth/Inp;
    WdgWidth ~ ./IncrW/Inp;
    IncrData ~ ./IncrH/Inp;
    ./IncrH/Out ~ ./WdgHeight/Inp;
    WdgHeight ~ ./IncrH/Inp;
    $ # "WdgWidth ~ ./Env/Window/Scene/Cnt1/InpAlcW";
    $ # "WdgHeight ~ ./Env/Window/Scene/Cnt1/InpAlcH";
}
