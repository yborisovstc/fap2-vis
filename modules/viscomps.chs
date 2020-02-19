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
    Heigth : AStatec;
    ./Heigth - Value = "SI 480";
  }
  Scene : Syst
  {
    About = "Visualization system scene";
    SceneAgt : AGtScene;
  }
  SceRect : Syst
  {
    About = "Scene element";
    Agent : ASceRect;
  }
}
