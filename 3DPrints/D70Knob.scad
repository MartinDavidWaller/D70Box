//
//  D70 Knob
//
//  Author:         M.D.Waller
//  Date:           February 27th, 2020
//
//  Date:           Modified By:    What:
//  =====           ============    =====
//
//  ?               ?               ?
//

cylinderResolution = 200;

module mainKnob()
{
    union()
    {
        // Build the top cap
        
        translate([0,0,18.5]) cylinder(r=8.8 / 2, h=0.5, $fn=cylinderResolution);
    
        // Build the main body of the knob
        
        translate([0,0,4]) cylinder(r1=(11.3 - 0.7) / 2, r2= (10 - 0.7) / 2, h=18.5 - 1 - 3, $fn=cylinderResolution);
    
        // Build the skirt recess
        
        translate([0,0,3]) cylinder(r=9.4 / 2, h=1, $fn=cylinderResolution);
    
        // Build the skirt
        
        translate([0,0,0]) cylinder(r=14 / 2, h=3, $fn=cylinderResolution);    
    }
}

module grip()
{
    difference()
    {
        translate([0,0,4]) cylinder(r1=11.3 / 2, r2= 10 / 2, h=18.5 - 1 - 3, $fn=cylinderResolution);
        
        for(i = [0 : 5]) 
        {
            rotate([0,0,i * 60]) translate([-0.5,0,0]) cube([1,10,20]);
        }
    }
}

module potShaft()
{
    union()
    {
        for(i = [0 : 19]) 
        {
            rotate([0,0,i * 20]) translate([0,-0.25 / 2,5]) cube([3.4,0.25,11]);
        }
        
        translate([0,0,5]) cylinder(r= 2.95, h=11, $fn=cylinderResolution);  
        
        translate([0,0,-1]) cylinder(r=4, h=6.3, $fn=cylinderResolution);
    }
}
 
// Build the Knob

difference()
{
    // Stick the grip to the main knob
    
    union()
    {
        grip();
        mainKnob();
    }
    
    // Remove the skirt marker
    
    translate([-0.5,6.4,-0.5]) cube([1,10,4]);
    translate([-0.5,6.4,2.6]) cube([1,10,3]);
    translate([-0.5,4.7,2.6]) cube([1,10,1]);

    // Remove the pot shaft
    
    potShaft();
}





