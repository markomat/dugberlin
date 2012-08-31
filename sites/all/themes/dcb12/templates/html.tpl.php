<!doctype html>
<!--[if IEMobile 7]><html class="no-js iem7" <?php print $html_attributes; ?>><![endif]-->
<!--[if (gt IEMobile 7)|!(IEMobile)]><!--><html class="no-js" lang="en"><!--<![endif]-->
<!--[if lt IE 7]><html class="no-js ie6 oldie" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 7)&!(IEMobile)]><html class="no-js ie7 oldie" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 8)&!(IEMobile)]><html class="no-js ie8 oldie" <?php print $html_attributes; ?>><![endif]-->
<!--[if gt IE 8]><!--><html class="no-js" <?php print $html_attributes; ?>><!--<![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)]><!--><html class="no-js"  <?php print $rdf_namespaces; ?>><!--<![endif]-->
<?php print $mothership_poorthemers_helper; ?>
<head>
<title><?php print $head_title; ?></title>
<?php print $head; ?>
<?php print $appletouchicon; ?>
<?php if(theme_get_setting('mothership_mobile')){  ?>
<meta name="MobileOptimized" content="width">
<meta name="HandheldFriendly" content="true"><?php } ?>
<?php if(theme_get_setting('mothership_viewport')){  ?><meta name="viewport" content="width=device-width, initial-scale=1"><?php } ?>
<?php if(theme_get_setting('mothership_viewport_maximumscale')){  ?><meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1.0"><?php } ?>
<meta http-equiv="cleartype" content="on">
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
<?php print $styles; ?>

<?php /* Einbindung der Styles vor Launch unbedingt ändern !! */ ?>
<link rel="stylesheet/less" type="text/css" href="<?php print base_path(); ?>sites/all/themes/dcb12/less/style.less">
<script src="<?php print base_path(); ?>sites/all/themes/dcb12/js/less-1.3.0.min.js"></script>
<?php /* end LESS */ ?>


<?php if(theme_get_setting('mothership_respondjs')) { ?>
<!--[if lt IE 9]>
  <script src="<?php print $mothership_path; ?>/js/respond.min.js"></script>
<![endif]-->
<?php } ?>
<!--[if lt IE 9]>
  <script src="<?php print $mothership_path; ?>/js/html5.js"></script>
<![endif]-->
<?php print $selectivizr; ?>
<?php
  if(!theme_get_setting('mothership_script_place_footer')) {
    print $scripts;
  }
?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
<a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
<?php print $page_top; //stuff from modules always render first ?>
<?php print $page; // uses the page.tpl ?>
<?php
  if(theme_get_setting('mothership_script_place_footer')) {
    print $scripts;
  }
?>
<?php print $page_bottom; //stuff from modules always render last ?>
</body>
</html>

