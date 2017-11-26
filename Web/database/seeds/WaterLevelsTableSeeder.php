<?php

use Carbon\Carbon;
use Illuminate\Database\Seeder;

class WaterLevelsTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        \DB::table('water_levels')->truncate();

        $d = Carbon::now()->subDay();
        $now = Carbon::now();

        $level = 100.0;
        $sub = true;

        while ($d <= $now)
        {
            \DB::table('water_levels')->insert([
                'level' => $level,
                'timestamp' => $d,
            ]);

            $d->addMinutes(10);

            if ($level < 30)
            {
                $sub = false;
            }
            else if ($level > 80)
            {
                $sub = true;
            }

            $level += ($sub ? -1 : 1) * min(random_int(0, 5), random_int(2, 5), random_int(1, 5));
        }
    }
}
