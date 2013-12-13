<?php

/**
 * @file
 * Syndication logic here.
 */

/**
 * TODO A foo?
 * Returns HTML for a foo.
 *
 * @param string $type
 *   The type of representation. It will be either 'ajax'
 *   or 'nojs'.
 *
 * @param string $bpi_id
 *   The BPI node ID.
 *
 * @return html
 *   Either an ajax-popup or an HTML representation of the item's
 *   preview.
 */
function admin_bpi_preview_ajax_callback($type, $bpi_id) {
  if (empty($bpi_id)) {
    return _admin_bpi_preview_output($type, t('Incorrect BPI ID.'));
  }
  $bpi_ctype = variable_get('bpi_content_type', '');
  // This could lead to unexpected behavior. Just a note.
  if (empty($bpi_ctype)) {
    return;
  }

  try {
    $bpi = bpi_client_instance();
    $bpi_node = $bpi->getNode($bpi_id);
    $bpi_content = $bpi_node->getProperties();
  }
  catch (Exception $e) {
    watchdog_exception('bpi', $e);
    return _admin_bpi_preview_output($type, t('Failed to fetch the article from BPI well.'));
  }

  $output = theme('bpi_preview_item', array('item' => $bpi_content));
  if ($type == 'ajax') {
    $commands = array();
    $commands[] = ajax_command_ding_popup(
      'bpi-push',
      t('Push to BPI'),
      $output,
      array('refresh' => TRUE)
    );
    $page = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($page);
  }
  else {
    return $output;
  }
}
