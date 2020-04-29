GVisComps : Elem
{
    $ + /SysComps;
    $ + /DesComps;
    About = "Visualization system based on GLUT";
    VisEnv : Syst
    {
        About = "Visualization system environment";
        VisEnvAgt : AVisEnv;
    }
    SceneCp : ASocketMcm
    {
        Width : CpStatecOutp;
        Height : CpStatecOutp;
    }
    SceneCpc : ASocketMcm
    {
        Width : CpStatecInp;
        Height : CpStatecInp;
    }
    GWindow : Syst
    {
        About = "Top-level window";
        AWnd : AGWindow;
        Inp_X : CpStatecInp;
        Inp_Y : CpStatecInp;
        Inp_W : CpStatecInp;
        Inp_H : CpStatecInp;
        Inp_Title : CpStatecInp;
        Width : AStatec;
        ./Width - Value = "SI 640";
        Height : AStatec;
        ./Height - Value = "SI 480";
        ScCpc : ./../SceneCpc;
        ./ScCpc/Width ~ Width;
        ./ScCpc/Height ~ Height;
    }
    Scene : Syst
    {
        About = "Visualization system scene";
        SceneAgt : AGtScene;
        Cp : ./../SceneCp;
    }
    SceRect : Syst
    {
        About = "Scene element";
        Agent : ASceRect;
    }
}
