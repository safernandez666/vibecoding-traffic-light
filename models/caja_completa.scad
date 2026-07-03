// Caja completa: modulo semaforo (3 LEDs) + buzzer + ESP32 adentro
// con orificio USB-C, clip al monitor, y tapa trasera.
// >>> AJUSTAR: led_pitch, led_d, mon_thick, usb_w/usb_y <<<
$fn=64;
part = "base";  // "base" o "tapa"

// modulo (3 LEDs)
led_d=10.5; led_pitch=13; led_top=14; module_w=16;
// buzzer
buzz_d=12.5; spk_hole_d=1.6; spk_from_bot=16;
// ESP32 (plana contra la pared trasera)
esp_thk=13;          // alto de la ESP con componentes
// caja
inner_w=34; inner_h=88; inner_d=33;   // +8mm de camara para cables soldados
wall=2.4; front_wall=2.2;
// USB-C (ranura en la base)
usb_w=12; usb_len=12; usb_y=6;   // usb_y = distancia desde la pared trasera
// clip monitor
mon_thick=16; tab_thk=3; front_drop=10; back_drop=20;

out_w=inner_w+2*wall; out_d=wall+inner_d+front_wall; out_h=inner_h+2*wall;
fy=out_d-front_wall; cx=out_w/2; bx0=cx-module_w/2;

module led_holes(){ for(i=[0:2]){ z=wall+inner_h-led_top-i*led_pitch;
  translate([cx,fy-1,z]) rotate([-90,0,0]) cylinder(h=front_wall+2,d=led_d);} }
module grille(){ cz=spk_from_bot;
  translate([cx,fy-1,cz]) rotate([-90,0,0]) cylinder(h=front_wall+2,d=spk_hole_d);
  for(a=[0:60:300]) for(r=[3,6])
    translate([cx+r*cos(a),fy-1,cz+r*sin(a)]) rotate([-90,0,0]) cylinder(h=front_wall+2,d=spk_hole_d); }
module body(){ difference(){
  cube([out_w,out_d,out_h]);
  translate([wall,-1,wall]) cube([inner_w,wall+inner_d+1,inner_h]);   // cavidad, abierta atras
  led_holes(); grille();
  // ranura USB-C en la base (pared inferior z=0..wall)
  translate([cx-usb_w/2, wall+usb_y, -1]) cube([usb_w, usb_len, wall+2]);
} }
module clip(){ front_y=out_d-tab_thk; back_y=front_y-mon_thick-tab_thk;
  translate([0,front_y,-front_drop]) cube([out_w,tab_thk,front_drop+0.5]);
  translate([0,back_y,-back_drop]) cube([out_w,tab_thk,back_drop+0.5]); }
// encastre modulo (canal frontal-inferior)
module module_channel(){ ch_h=7; th=2.4; my=fy-9;
  difference(){
    translate([bx0-2,my,wall]) cube([module_w+4,th+2.8,ch_h]);
    translate([bx0-0.2,my+1.4-0.1,wall+1.6]) cube([module_w+0.4,th,ch_h]); } }
// copa buzzer
module buzzer_cup(){ cl=7;
  difference(){
    translate([cx,fy-cl,spk_from_bot]) rotate([-90,0,0]) cylinder(h=cl,d=buzz_d+4);
    translate([cx,fy-cl-1,spk_from_bot]) rotate([-90,0,0]) cylinder(h=cl+2,d=buzz_d+0.4); } }
// tope de la ESP: repisas que la empujan al fondo (queda plana contra la pared trasera)
module esp_stops(){ for(x=[wall+2, out_w-wall-6])
  translate([x,wall+esp_thk,wall]) cube([4,2.4,inner_h-6]); }

module base(){ color("Khaki"){ body(); clip(); module_channel(); buzzer_cup(); esp_stops(); } }

// tapa trasera (cierra la parte de atras despues de soldar y meter la ESP)
module tapa(){ clr=0.4; lip=6; bt=2.4;
  color("Salmon"){
  difference(){
    union(){
      cube([out_w, bt, out_h]);   // placa trasera (en el plano y=0)
      translate([wall+clr, bt, wall+clr])
        difference(){
          cube([inner_w-2*clr, lip, inner_h-2*clr]);
          translate([2.4,-1,2.4]) cube([inner_w-2*clr-4.8, lip+2, inner_h-2*clr-4.8]);
        }
    }
    // ranura USB-C tambien en el borde de la tapa
    translate([cx-usb_w/2, -1, wall]) cube([usb_w, bt+2, 1]);
  } }
}

if(part=="base") base(); else tapa();
