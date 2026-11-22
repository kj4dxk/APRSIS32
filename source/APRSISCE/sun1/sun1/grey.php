<?php
header('Content-Type: image/jpeg');
$target="greyline.jpg";
$diff=0;
$maxtime=300;
$maj=0;
if (is_file($target)){
	$t=time();
	clearstatcache();
	$date_fic=filemtime($target);
	$diff=$t-$date_fic;
	if ($diff>$maxtime){
			$maj=1;
	}
}else{
	$maj=1;
}

function grey($Longitude,$Latitude,$day,$time){
		
		#Longitude en deg Est positif
		#Latitude en deg Nord positif
		$d=$day;
		$hour=$time;
		$j=1;
		$g=(360/365.25)*($d + $hour/24);
		$G=deg2rad($g);
		#Déclinaison
		$D=0.396372-22.91327*cos($G)+4.02543*sin($G)-0.387205*cos(2*$G)+0.051967*sin(2*$G)-0.154527*cos(3*$G) + 0.084798*sin(3*$G); 
		#Solar time
		$TC = 0.004297+0.107029*cos($G)-1.837877*sin($G)-0.837378*cos(2*$G)-2.340475*sin(2*$G);
		#Solar Hour Angle (SHA)
		$SHA = ($hour-12)*15 + $Longitude + $TC;
		if ($SHA>180){$SHA=$SHA-360;}
		if ($SHA<-180){$SHA=$SHA+360;}
		#Sun Zenith Angle (SZA)
		$ASZA = sin(deg2rad($Latitude))*sin(deg2rad($D))+cos(deg2rad($Latitude))*cos(deg2rad($D))*cos(deg2rad($SHA));
		if ($ASZA>1){ $ASZA=1;}
		if ($ASZA<-1){ $ASZA=-1;}
		$SZA=rad2deg(acos($ASZA));
		//echo $SZA;
		/*
		if ($SZA>92){$j=0;}
		if ($SZA<-2){$j=0;}
		*/
		if ($SZA>91){
			$j=0;
		}
		if ($SZA>89 && $SZA<=91){
			$j=2;
		}
		if ($SZA<-1){
			$j=0;
		}
		if ($SZA<=-1 && $SZA<=1){
			$j=2;
		}
return($j);
}


function conv_lon($i,$un_deg_e){
	$long=$i*$un_deg_e-180;
return($long);
}
function conv_lat($i,$un_deg_s){
	$lat=90-$i*$un_deg_s;
return($lat);
}


if ($maj==1){

	$file="earth1.jpg";
	$file_nuit="earth2.jpg";

	$now=time();
	$tz=date("Z");
	$now=$now-$tz;
	$day=date("z",$now);
	$hour=date("G",$now);
	$minute=date("i",$now);
	$hour=$hour+($minute/60);

	$lon=-87; #Est positif
	$lat=48.81667; #Nord positif

	$im=imagecreatefromjpeg($file);
	$im_nuit=imagecreatefromjpeg($file_nuit);
	$black=ImageColorAllocate($im,0,0,0);


	# Reglage en fonction de la taille de l'image
	$largeur=imagesx($im);
	$hauteur=imagesy($im);
	$un_deg_e=360/$largeur;
	$un_deg_s=180/$hauteur;

	$i=1;
	while($i<$largeur){
		$p=1;
		$lon=conv_lon($i,$un_deg_e);
		while ($p<$hauteur){
			$lat=conv_lat($p,$un_deg_s);
			$jour=grey($lon,$lat,$day,$hour);
			if ($jour==0){
			 imagecopy  ($im , $im_nuit , $i , $p , $i , $p, 1 , 1 );
			}
			if ($jour==2){
			$pixelrgb = imagecolorat($im,$i,$p);
			$cols = imagecolorsforindex($im, $pixelrgb);
			$r = $cols['red']-30;
			$g = $cols['green']-30;
			$b = $cols['blue'];		
			$rgb=imagecolorallocatealpha ($im, $r ,$g, $b, 0) ;
			imagesetpixel($im,$i,$p,$rgb);
			}
		$p++;
		}
		$i++;
	}
	Imagejpeg($im,$target,85);
	ImageDestroy($im);
	$im=Imagecreatefromjpeg($target);
	Imagejpeg($im,null,85);
	imagedestroy($im);
}else {
	$im=Imagecreatefromjpeg($target);
	Imagejpeg($im,null,100);
	imagedestroy($im);
}
?>

