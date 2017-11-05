<?php

namespace App\Http\Controllers;

use App\Http\Resources\WaterLevelResource;
use App\WaterLevel;
use Carbon\Carbon;

class WaterLevelController extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        $levels = WaterLevel::where('timestamp', '>=', Carbon::now()->subDay(1))
                            ->get();

        return WaterLevelResource::collection($levels->values());
    }
}
