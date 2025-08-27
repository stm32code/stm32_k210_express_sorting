package com.example.expresssortingsystemdesign.bean;

public class Send {
    private Integer cmd;
    private Integer cityid;
    private Integer area;
    private Integer id;
    private Integer state;

    @Override
    public String toString() {
        return "Send{" +
                "cmd=" + cmd +
                ", cityid=" + cityid +
                ", area=" + area +
                ", id=" + id +
                ", state=" + state +
                '}';
    }

    public Integer getCmd() {
        return cmd;
    }

    public void setCmd(Integer cmd) {
        this.cmd = cmd;
    }

    public Integer getCityid() {
        return cityid;
    }

    public void setCityid(Integer cityid) {
        this.cityid = cityid;
    }

    public Integer getArea() {
        return area;
    }

    public void setArea(Integer area) {
        this.area = area;
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getState() {
        return state;
    }

    public void setState(Integer state) {
        this.state = state;
    }
}
