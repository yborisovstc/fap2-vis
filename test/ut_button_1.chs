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
        $ + /FvWidgets/FButton;
        $ + /ContainerMdl/HLayout;
    }
    Launcher : AVDesLauncher;
    Launcher < Path = /testroot/Test;
    Comps : Elem;
    Test : /testroot/Modules/DesComps/Des
    {
        $ # "Visualisation environment";
        VisEnvAgt : AVisEnv;
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
                    Slot_1 < Btn1 : /*/Modules/FvWidgets/FButton
                    {
                        Text = "Button 1";
                        BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                        FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                    }
                    Slot_2 : AVSlot;
                    Slot_2 < Btn2 : /*/Modules/FvWidgets/FButton
                    {
                        Text = "Button 2";
                        BgColor = "{R:'0.0' G:'0.0' B:'1.0'}";
                        FgColor = "{R:'1.0' G:'1.0' B:'1.0'}";
                    }
                }
            }
        }
        EnvWidth : AStatec;
        EnvHeight : AStatec;
        Title : AStatec;
        EnvWidth ~ ./Env/Window/Inp_W;
        EnvHeight ~ ./Env/Window/Inp_H;
        Title ~ ./Env/Window/Inp_Title;
        $ # " Initialization";
        EnvWidth < Value = "SI 640";
        EnvHeight < Value = "SI 480";
        Title < Value = "SS Title";
    }
}
