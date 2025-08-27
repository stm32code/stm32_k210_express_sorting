package com.example.expresssortingsystemdesign.bean;

public class Receive {
    private String cityid;
    private String area;
    private String id;
    private String state;

    @Override
    public String toString() {
        return "Receive{" +
                "cityid='" + cityid + '\'' +
                ", area='" + area + '\'' +
                ", id='" + id + '\'' +
                ", state='" + state + '\'' +
                '}';
    }

    public String getCityid() {
        return cityid;
    }

    public void setCityid(String cityid) {
        this.cityid = cityid;
    }

    public String getArea() {
        return area;
    }

    public void setArea(String area) {
        this.area = area;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }
}
