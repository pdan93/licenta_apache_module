<?php
$ip = explode('.',$argv[1]);
$ip = implode('.',array_reverse($ip));
$result = dns_get_record("wjrkudvgxlao.".$ip.".dnsbl.httpbl.org");

if (isset($result[0]))
	if (isset($result[0]['ip']))
		{
		$r = explode('.',$result[0]['ip']);
		if ($r[3]>0)
			{
			$map = array(
				1 => 'Suspicious',
				2 => 'Harvester ',
				3 => 'Suspicious & Harvester',
				4 => 'Comment Spammer',
				5 => 'Suspicious & Comment Spammer',
				6 => 'Harvester & Comment Spammer',
				7 => 'Suspicious & Harvester & Comment Spammer',
			);
			if (isset($map[$r[3]]))
				echo $map[$r[3]]."\r\n";
				else
				echo $r[3]."\r\n";
			echo $r[2]."\r\n";
			}
			else
			{
			echo "0"."\r\n";
			echo "0"."\r\n";
			}
		}
			else
			{
			echo "0"."\r\n";
			echo "0"."\r\n";
			}
			else
			{
			echo "0"."\r\n";
			echo "0"."\r\n";
			}
?>