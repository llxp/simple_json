function Compare-String {
  param(
    [String] $string1,
    [String] $string2
  )
  if ( $string1 -ceq $string2 ) {
    return -1
  }
  for ( $i = 0; $i -lt $string1.Length; $i++ ) {
    if ( $string1[$i] -cne $string2[$i] ) {
      return $i
    }
  }
  return $string1.Length
}

$str1 = Get-Content -Path "secrets.txt" | Out-String
$str2 = Get-Content -Path "secrets2.txt" | Out-String
$pos = Compare-String -string1 $str1 -string2 $str2
$pos
$counter = 0;
$lines = 0;
for($i =0;$i -lt $pos;$i++) {
if($str1[$i] -eq "`n") {
$lines++;
}
}
$lines