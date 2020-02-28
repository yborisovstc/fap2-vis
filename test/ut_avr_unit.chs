testroot : Elem
{
    $ # "Unit test of agents visual representation, unit";
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
                    Wdg1 : /*/Modules/FvWidgets/FUnitCrp
                    {
                        BgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                    }
                    ./Wdg1/AlcW < Value = "SI 80";
                    ./Wdg1/AlcH < Value = "SI 100";
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
        $ # "IncrData ~ ./IncrW/Inp";
        ./IncrW/Out ~ ./WdgWidth/Inp;
        WdgWidth ~ ./IncrW/Inp;
        $ # "IncrData ~ ./IncrH/Inp";
        ./IncrH/Out ~ ./WdgHeight/Inp;
        WdgHeight ~ ./IncrH/Inp;
        WdgWidth ~ ./Env/Window/Scene/Wdg1/InpAlcW;
        WdgHeight ~ ./Env/Window/Scene/Wdg1/InpAlcH;
    }
}
