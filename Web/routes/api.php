<?php

use Illuminate\Http\Request;

/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| is assigned the "api" middleware group. Enjoy building your API!
|
*/

Route::group(['prefix' => 'water-levels'], function()
{
    Route::get('', 'WaterLevelController@index');
});

Route::get('variables', 'VariableController@index');
Route::put('/variables/{variable}', 'VariableController@update');