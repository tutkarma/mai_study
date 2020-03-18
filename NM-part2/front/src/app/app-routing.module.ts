import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Routes, RouterModule } from '@angular/router';
import { Lab5Component } from './lab5/lab5.component';
import { Lab6Component } from './lab6/lab6.component';
import { Lab7Component } from './lab7/lab7.component';
import { Lab8Component } from './lab8/lab8.component';


const routes: Routes = [
    {path: '', redirectTo: '/lab5', pathMatch: 'full'},
    {path: 'lab5', component: Lab5Component},
    {path: 'lab6', component: Lab6Component},
    {path: 'lab7', component: Lab7Component},
    {path: 'lab8', component: Lab8Component},
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
