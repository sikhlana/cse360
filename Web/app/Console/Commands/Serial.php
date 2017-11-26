<?php

namespace App\Console\Commands;

use App\Variable;
use App\WaterLevel;
use Carbon\Carbon;
use Illuminate\Console\Command;

class Serial extends Command
{
    /**
     * The name and signature of the console command.
     *
     * @var string
     */
    protected $signature = 'serial';

    /**
     * The console command description.
     *
     * @var string
     */
    protected $description = 'Communicates with the Arduino for monitoring the water level and temperatures.';

    /**
     * Create a new command instance.
     */
    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Execute the console command.
     *
     * @return mixed
     */
    public function handle()
    {
        $handle = @fopen('php://stdin', 'rb');
        list ($temp, $level) = explode('|', trim(fgets($handle)));
        $output = [];

        /** @var Variable $pump */
        $pump = Variable::findOrFail('pump');
        /** @var Variable $currentTemp */
        $currentTemp = Variable::findOrFail('current_temperature');
        /** @var Variable $cooler */
        $cooler = Variable::findOrFail('cooler');
        /** @var Variable $heater */
        $heater = Variable::findOrFail('heater');

        $level = floatval($level);

        if ($level < 0)
        {
            $level = 0;
        }
        else if ($level > 100)
        {
            $level = 100;
        }

        if ($level < 30)
        {
            $output[] = 'pi';
            $pump->value = 'on';
        }
        else if ($level >= 80)
        {
            $output[] = 'po';
            $pump->value = 'off';
        }

        /** @var WaterLevel $latest */
        $latest = WaterLevel::orderByDesc('id')->first();

        if ($latest->timestamp <= Carbon::now()->subMinutes(10))
        {
            WaterLevel::create([
                'level' => $level,
                'timestamp' => Carbon::now(),
            ]);
        }

        $temp = floatval($temp);
        $currentTemp->value = $temp;

        $desired = floatval(Variable::findOrFail('preferred_temperature')->value);
        $desireHigh = $desired + 5;
        $desiredLow = $desired - 5;

        if ($temp < $desiredLow)
        {
            $output[] = 'hi';
            $heater->value = 'on';
        }
        if ($temp >= $desired)
        {
            $output[] = 'ho';
            $heater->value = 'off';
        }

        if ($temp > $desireHigh)
        {
            $output[] = 'ci';
            $cooler->value = 'on';
        }
        if ($temp <= $desired)
        {
            $output[] = 'co';
            $cooler->value = 'off';
        }

        $pump->save();
        $cooler->save();
        $heater->save();
        $currentTemp->save();

        echo implode('|', $output);
        return null;
    }
}
