import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Routes, RouterModule } from '@angular/router';
import { Lab5Component } from './lab5/lab5.component';


const routes: Routes = [
    {path: '', redirectTo: '/lab5', pathMatch: 'full'},
    {path: 'lab5', component: Lab5Component},
];

@NgModule({
  declarations: [],
  imports: [
      RouterModule.forRoot(routes),
      CommonModule,
  ],
  exports: [
      RouterModule
  ]
})
export class AppRoutingModule {
}
