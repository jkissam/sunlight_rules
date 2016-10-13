<?php

/**
 * implements hook_rules_action_info
 * creates custom action for rules
 */
function sunlight_rules_rules_action_info() {

	return array(
		'sunlight_rules_get_state_legislators' => array(
			'label' => t('Get state legislators by lat/lng coordinates'),
			'group' => t('Sunlight Rules'),
			'parameter' => array(
				'latitude' => array(
					'label' => t('Latitude'),
					'type' => 'text',
				),
				'longitude' => array(
					'label' => t('Longitude'),
					'type' => 'text',
				),
			),
			'provides' => array(
				'sunlight_state_legislators_dump' => array(
					'type' => 'text',
					'label' => t('Legislators - Variable Dump'),
				),
			),
			'access callback' => '_sunlight_rules_access_callback',
		),
		
		'sunlight_rules_get_state_legislator_districts' => array(
			'label' => t('Get state legislator districts by lat/lng coordinates'),
			'group' => t('Sunlight Rules'),
			'parameter' => array(
				'latitude' => array(
					'label' => t('Latitude'),
					'type' => 'text',
				),
				'longitude' => array(
					'label' => t('Longitude'),
					'type' => 'text',
				),
			),
			'provides' => array(
				'sunlight_state_lower_chamber_district' => array(
					'type' => 'text',
					'label' => t('Lower Chamber District'),
				),
				'sunlight_state_upper_chamber_district' => array(
					'type' => 'text',
					'label' => t('Upper Chamber District'),
				),
			),
			'access callback' => '_sunlight_rules_access_callback',
		),
	);

}

/**
 * callback function for getting state legislators
 */
function sunlight_rules_get_state_legislators($latitude, $longitude) {

	$return = array(
		'sunlight_state_legislators_dump' => null,
	);
	
	$state_legislators = sunlight('OpenStates','geo_lookup');
	$state_legislators_object = $state_legislators->coords($latitude, $longitude)->get();
	
	$return['sunlight_state_legislators_dump'] = print_r($state_legislators_object,1);

	return $return;

}

/**
 * callback function for getting state legislator districts
 */
function sunlight_rules_get_state_legislator_districts($latitude, $longitude) {

	$return = array(
		'sunlight_state_upper_chamber_district' => null,
		'sunlight_state_lower_chamber_district' => null,
	);
	
	$state_legislators = sunlight('OpenStates','geo_lookup');
	$state_legislators_object = $state_legislators->coords($latitude, $longitude)->get();
	
	foreach($state_legislators_object as $state_legislator) {
		if ( isset($state_legislator->chamber) && isset($state_legislator->district) ) {
			if ( $state_legislator->chamber == 'upper') {
				$return['sunlight_state_upper_chamber_district'] = $state_legislator->district;
			} elseif ( $state_legislator->chamber == 'lower') {
				$return['sunlight_state_lower_chamber_district'] = $state_legislator->district;
			}			
		}
	}

	return $return;

}



/**
 * helper function to provide access control
 */

function _sunlight_rules_access_callback() {
	return user_access('use sunlight rules');
}


/**
 * helper function to log requests
 */
function _sunlight_rules_log_request($log) {

/*
	$object = new stdClass();
	$object->timestamp = time();
	$fields = array('hook','url','method','headers','data','callback','status_code',
		'status_method','error','data_returned');
	foreach ($fields as $field) {
		$object->$field = isset($log[$field]) ? $log[$field] : null;
	}

	watchdog('rules_api', '<pre>'.print_r($object, 1).'</pre>');
	*/
}
